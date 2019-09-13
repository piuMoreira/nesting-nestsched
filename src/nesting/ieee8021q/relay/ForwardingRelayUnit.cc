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

#include "ForwardingRelayUnit.h"

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"

namespace nesting {

Define_Module(ForwardingRelayUnit);

void ForwardingRelayUnit::initialize(int stage) {
    if (stage == INITSTAGE_LOCAL) {
        fdb = getModuleFromPar<FilteringDatabase>(par("filteringDatabaseModule"), this);
        ifTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
        numberOfPorts = par("numberOfPorts");
    } else if (stage == INITSTAGE_LINK_LAYER) {
        registerService(Protocol::ethernetMac, nullptr, gate("ifIn"));
        registerProtocol(Protocol::ethernetMac, gate("ifOut"), nullptr);
    }
}

void ForwardingRelayUnit::handleMessage(cMessage *msg) {
    Packet* packet = check_and_cast<Packet*>(msg);
    const auto& frame = packet->peekAtFront<EthernetMacHeader>();

    // Distinguish between broadcast-, multicast- and unicast-ethernet frames
    if (frame->getDest().isBroadcast()) {
        processBroadcast(packet);
    } else if (frame->getDest().isMulticast()) {
        processMulticast(packet);
    } else {
        processUnicast(packet);
    }
}

void ForwardingRelayUnit::processBroadcast(Packet* packet) {
    // Flood packets everywhere except of ingress port
    // TODO this is just a temporary solution not sure how correct that is
    for (int portId = 0; portId < gateSize("out"); portId++) {
        cGate *outputGate = gate("out", portId);
        if (!packet->arrivedOn("in", portId)) {
            Packet* dupPacket = packet->dup();
            send(dupPacket, outputGate);
        }
    }
    delete packet;
}

void ForwardingRelayUnit::processMulticast(Packet* packet) {
    const auto& frame = packet->peekAtFront<EthernetMacHeader>();
    int arrivalGate = packet->getArrivalGate()->getIndex();

    std::vector<int> forwardingPorts = fdb->getPorts(frame->getDest(), simTime());

    if (forwardingPorts.at(0) == -1) {
        throw cRuntimeError(
                "Static multicast forwarding for packet didn't work. Entry in forwarding table was empty!");
    } else {
        std::string forwardingPortsString = "";
        for (auto forwardingPort : forwardingPorts) {
            // skip arrival gate
            if (forwardingPort == arrivalGate) {
                continue;
            }
            Packet* dupPacket = packet->dup();
            send(dupPacket, gate("out", forwardingPort));
            forwardingPortsString = forwardingPortsString.append(
                    std::to_string(forwardingPort));
        }
        EV_INFO << getFullPath() << ": Forwarding multicast packet `" << packet << "` to ports "
                << forwardingPortsString << endl;
    }
    delete packet;
}

void ForwardingRelayUnit::processUnicast(Packet* packet) {
    //Learning MAC port mappings
    const auto& frame = packet->peekAtFront<EthernetMacHeader>();
    fdb->insert(frame->getSrc(), simTime(), packet->getArrivalGate()->getIndex());
    int forwardingPort = fdb->getPort(frame->getDest(), simTime());

    Packet* dupPacket;
    //Routing entry available?
    if (forwardingPort == -1) {
        dupPacket = packet->dup();
        processBroadcast(dupPacket);
        EV_INFO << getFullPath() << ": Broadcasting packets `" << packet << "` to all ports" << endl;
    } else {
        dupPacket = packet->dup();
        EV_INFO << getFullPath() << ": Forwarding packet `" << packet << "` to port " << forwardingPort << endl;
        send(dupPacket, gate("out", forwardingPort));
    }

    delete packet;
}

} // namespace nesting
