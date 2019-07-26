//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "VlanEtherTrafGenSched.h"
#include <algorithm>

#define COMPILETIME_LOGLEVEL omnetpp::LOGLEVEL_TRACE

namespace nesting {

Define_Module(VlanEtherTrafGenSched);

VlanEtherTrafGenSched::~VlanEtherTrafGenSched() {
    for (cMessage* msg : jitterMsgVector) {
        cancelEvent(msg);
        delete (msg);
    }
    jitterMsgVector.clear();
}

void VlanEtherTrafGenSched::initialize(int stage) {
    if (stage == INITSTAGE_LOCAL) {
        // Signals
        sentPkIdSignal = registerSignal("sentPkId");
        rcvdPkIdSignal = registerSignal("rcvdPkId");
        sentPkSignal = registerSignal("sentPk");
        rcvdPkSignal = registerSignal("rcvdPk");

        seqNum = 0;
        //WATCH(seqNum);

        jitter = par("jitter");
        seed = par("seed");
        // set seed for random jitter calculation
        srand(seed);

        // statistics
        TSNpacketsSent = packetsReceived = 0;
        WATCH(TSNpacketsSent);
        WATCH(packetsReceived);

        cModule* clockModule = getModuleFromPar<cModule>(par("clockModule"),
                this);
        clock = check_and_cast<IClock*>(clockModule);

        llcSocket.setOutputGate(gate("out"));
    } else if (stage == INITSTAGE_LINK_LAYER) {
        //clock module reference from ned parameter

        currentSchedule = std::unique_ptr < HostSchedule
                < Ieee8021QCtrl >> (new HostSchedule<Ieee8021QCtrl>());
        cXMLElement* xml = par("initialSchedule").xmlValue();
        loadScheduleOrDefault(xml);

        currentSchedule = move(nextSchedule);
        nextSchedule.reset();

        clock->subscribeTick(this,
                scheduleNextTickEvent().raw() / clock->getClockRate().raw());

        llcSocket.open(-1, ssap);
    }
}

int VlanEtherTrafGenSched::numInitStages() const {
    return INITSTAGE_LINK_LAYER + 1;
}

void VlanEtherTrafGenSched::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        sendDelayed(msg);
    } else {
        receivePacket(check_and_cast<Packet *>(msg));
    }
}

void VlanEtherTrafGenSched::sendPacket() {

    char msgname[40];
    sprintf(msgname, "pk-%d-%d", getId(), seqNum);

    // create new packet
    Packet *datapacket = new Packet(msgname, IEEE802CTRL_DATA);
    long len = currentSchedule->getSize(indexTx % currentSchedule->size());
    const auto& payload = makeShared<ByteCountChunk>(B(len));
    // set creation time
    auto timeTag = payload->addTag<CreationTimeTag>();
    timeTag->setCreationTime(simTime());

    datapacket->insertAtBack(payload);
    datapacket->removeTagIfPresent<PacketProtocolTag>();
    datapacket->addTagIfAbsent<PacketProtocolTag>()->setProtocol(
            &Protocol::ethernetMac);
    // TODO check if protocol is correct
    auto sapTag = datapacket->addTagIfAbsent<Ieee802SapReq>();
    sapTag->setSsap(ssap);
    sapTag->setDsap(dsap);

    seqNum++;

    // get scheduled control data
    Ieee8021QCtrl header = currentSchedule->getScheduledObject(
            indexTx % currentSchedule->size());
    // create mac control info
    auto macTag = datapacket->addTag<MacAddressReq>();
    macTag->setDestAddress(header.macTag.getDestAddress());
    // create VLAN control info
    auto ieee8021q = datapacket->addTag<VLANTagReq>();
    ieee8021q->setPcp(header.q1Tag.getPcp());
    ieee8021q->setDe(header.q1Tag.getDe());
    ieee8021q->setVID(header.q1Tag.getVID());

    EV_TRACE << getFullPath() << ": Send TSN packet '" << datapacket->getName()
                    << "' at time " << clock->getTime().inUnit(SIMTIME_US)
                    << endl;

    emit(sentPkIdSignal, datapacket->getTreeId()); // getting tree id, because it doenn't get changed when packet is copied
    emit(sentPkSignal, datapacket);
    send(datapacket, "out");
    TSNpacketsSent++;
}

void VlanEtherTrafGenSched::receivePacket(Packet *msg) {
    EV_TRACE << getFullPath() << ": Received packet '" << msg->getName()
                    << "' with length " << msg->getByteLength() << "B at time "
                    << clock->getTime().inUnit(SIMTIME_US) << endl;

    packetsReceived++;
    emit(rcvdPkIdSignal, msg->getTreeId());
    emit(rcvdPkSignal, msg);

    delete msg;
}

void VlanEtherTrafGenSched::tick(IClock *clock) {
    Enter_Method("tick()");
    // When the current schedule index is 0, this means that the current
    // schedule's cycle was not started or was just finished. Therefore in this
    // case a new schedule is loaded if available.
    if (indexSchedule == currentSchedule->size() ) {
        // Load new schedule and delete the old one.
        if (nextSchedule) {
            currentSchedule = move(nextSchedule);
            nextSchedule.reset();
        }
        indexSchedule = 0;
        clock->subscribeTick(this, scheduleNextTickEvent().raw() / clock->getClockRate().raw());

    }
    else {
        double delay = (double)rand() / (double)RAND_MAX;
        simtime_t jitter_delay = delay * jitter;
        cMessage* jitterMsg = new cMessage();
        jitterMsgVector.push_back(jitterMsg);
        indexSchedule++;
        scheduleAt(simTime() + jitter_delay, jitterMsg);
        clock->subscribeTick(this, scheduleNextTickEvent().raw() / clock->getClockRate().raw());

    }
}

void VlanEtherTrafGenSched::sendDelayed(cMessage *msg) {
    sendPacket();
    indexTx++;
    std::vector<cMessage*>::iterator it = std::find(jitterMsgVector.begin(),
            jitterMsgVector.end(), msg);
    if (it != jitterMsgVector.end()) {
        jitterMsgVector.erase(it);
    } else {
        throw cRuntimeError("Jitter message not found in vector!");
    }

}

/* This method returns the timeinterval between
 * the last sent frame and the frame to be sent next */
simtime_t VlanEtherTrafGenSched::scheduleNextTickEvent() {
    if (currentSchedule->size() == 0) {
        return currentSchedule->getCycle();
    } else if (indexSchedule == currentSchedule->size()) {
        return currentSchedule->getCycle()
                - currentSchedule->getTime(indexSchedule - 1);
    } else if (indexSchedule % currentSchedule->size() == 0) {
        return currentSchedule->getTime(indexSchedule);
    } else {
        return currentSchedule->getTime(indexSchedule % currentSchedule->size())
                - currentSchedule->getTime(
                        indexSchedule % currentSchedule->size() - 1);
    }
}

void VlanEtherTrafGenSched::loadScheduleOrDefault(cXMLElement* xml) {
    std::string hostName =
            this->getModuleByPath(par("hostModule"))->getFullName();
    HostSchedule<Ieee8021QCtrl>* schedule;
    bool realScheduleFound = false;
    //try to extract the part of the schedule belonging to this host
    for (cXMLElement* hostxml : xml->getChildren()) {
        if (strcmp(hostxml->getTagName(), "defaultcycle") != 0
                && hostxml->getAttribute("name") == hostName) {
            schedule = HostScheduleBuilder::createHostScheduleFromXML(hostxml,
                    xml);

            EV_DEBUG << getFullPath() << ": Found schedule for name "
                            << hostName << endl;

            realScheduleFound = true;
            break;
        }
    }
    //load empty schedule if there is no part that affects this host in the schedule xml
    if (!realScheduleFound) {
        cXMLElement* defaultXml = par("emptySchedule").xmlValue();
        schedule = HostScheduleBuilder::createHostScheduleFromXML(defaultXml,
                xml);
    }
    std::unique_ptr<HostSchedule<Ieee8021QCtrl>> schedulePtr(schedule);

    nextSchedule.reset();
    nextSchedule = move(schedulePtr);

}

} // namespace nesting
