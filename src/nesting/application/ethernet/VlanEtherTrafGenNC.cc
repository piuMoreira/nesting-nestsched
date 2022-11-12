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

#include "nesting/linklayer/vlan/EnhancedVlanTag_m.h"
#include "nesting/application/ethernet/VlanEtherTrafGenSched.h"

#include "inet/common/TimeTag_m.h"
#include "inet/linklayer/common/Ieee802SapTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/Protocol.h"
#include "inet/common/Simsignals.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "VlanEtherTrafGenNC.h"

namespace nesting {

Define_Module(VlanEtherTrafGenNC);

int getPoisson(int lambda) {
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;

    do {
        k++;
        p *= ((double) rand() / (RAND_MAX));
    } while(p > L);

    return k-1;
}

void VlanEtherTrafGenNC::initialize(int stage) {
    EtherTrafGen::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        vlanTagEnabled = &par("vlanTagEnabled");
        pcp = &par("pcp");
        dei = &par("dei");
        vid = &par("vid");

        l_gen = &par("l_gen");
        l_len = &par("l_len");
        l_itv = &par("l_itv");
    } else if (stage == INITSTAGE_LINK_LAYER) {
        registerService(*VlanEtherTrafGenSched::L2_PROTOCOL, nullptr, gate("in"));
        registerProtocol(*VlanEtherTrafGenSched::L2_PROTOCOL, gate("out"), nullptr);
    }

}

void VlanEtherTrafGenNC::scheduleNextPacket(simtime_t previous)
{
    float x = (float) getPoisson(l_itv->intValue())/1000000;
    simtime_t next;
    if (previous == -1) {
        next = simTime() <= startTime ? startTime : simTime();
        timerMsg->setKind(START);
    }
    else {
        next = previous + ((simtime_t) x);
        timerMsg->setKind(NEXT);
    }
    if (stopTime < SIMTIME_ZERO || next < stopTime)
        scheduleAt(next, timerMsg);
}

void VlanEtherTrafGenNC::sendBurstPackets() {
    int n = getPoisson(l_gen->intValue());
    for (int i = 0; i < n; i++) {
        seqNum++;

        char msgname[40];
        sprintf(msgname, "%d-pk-%d-%ld", vid->intValue(), getId(), seqNum);

        // create new packet
        Packet *datapacket = new Packet(msgname, NONCRITICALFLOW);
        long len = getPoisson(l_len->intValue());
        const auto& payload = makeShared<ByteCountChunk>(B(len));
        // set creation time
        auto timeTag = payload->addTag<CreationTimeTag>();
        timeTag->setCreationTime(simTime());

        datapacket->insertAtBack(payload);
        datapacket->removeTagIfPresent<PacketProtocolTag>();
        datapacket->addTagIfAbsent<PacketProtocolTag>()->setProtocol(VlanEtherTrafGenSched::L2_PROTOCOL);
        // TODO check which protocol to insert
        auto sapTag = datapacket->addTagIfAbsent<Ieee802SapReq>();
        sapTag->setSsap(ssap);
        sapTag->setDsap(dsap);

        // create control info for encap modules
        auto macTag = datapacket->addTag<MacAddressReq>();
        macTag->setDestAddress(destMacAddress);

        // create VLAN control info
        if (vlanTagEnabled->boolValue()) {
            EnhancedVlanReq* vlanReq = datapacket->addTag<EnhancedVlanReq>();
            vlanReq->setPcp(pcp->intValue());
            vlanReq->setDe(dei->boolValue());
            vlanReq->setVlanId(vid->intValue());
        }

        EV_TRACE << getFullPath() << ": Send packet `" << datapacket->getName()
                        << "' dest=" << macTag->getDestAddress() << " length="
                        << datapacket->getBitLength() << "B type="
                        << NONCRITICALFLOW << " vlan-tagged="
                        << vlanTagEnabled->boolValue();
        if (vlanTagEnabled->boolValue()) {
            EV_TRACE << " pcp=" << pcp->intValue() << " dei=" << dei->boolValue() << " vid=" << vid->intValue();
        }
        EV_TRACE << endl;

        emit(packetSentSignal, datapacket);
        send(datapacket, "out");
        packetsSent++;
    }
}

} // namespace nesting
