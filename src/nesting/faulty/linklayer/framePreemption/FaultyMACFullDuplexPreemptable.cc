#include "nesting/faulty/linklayer/framePreemption/FaultyMACFullDuplexPreemptable.h"

#include "nesting/faulty/common/distributions/Distributions.h"

namespace nesting {

Define_Module(FaultyMACFullDuplexPreemptable);

Distributions distri;

FaultyMACFullDuplexPreemptable::~FaultyMACFullDuplexPreemptable() {

}

void FaultyMACFullDuplexPreemptable::initialize(int stage) {
    EtherMACFullDuplexPreemptable::initialize(stage);

    dropProbabilityPar = &par("dropProbability");
    probDistribution = &par("probDistribution");
}

void FaultyMACFullDuplexPreemptable::handleMessageWhenUp(cMessage *msg) {
    int isFaulty = distri.distributionRun(probDistribution->intValue(), 100);
    if (msg->arrivedOn("upperLayerIn") && isFaulty <= dropProbabilityPar->intValue() && dropProbabilityPar->intValue() != 0){
            Packet* packet = check_and_cast<Packet *>(msg);
            EV_WARN
                           << " -- dropping packet " << packet << endl;
            PacketDropDetails details;
            //details.setReason(INTERFACE_DOWN);
            //emit(packetDroppedSignal, packet->getTreeId(), &details);
            numDroppedPkFromHLIfaceDown++;
            delete packet;

            //requestNextFrameFromExtQueue();
            return;
    } else {
        EtherMACFullDuplexPreemptable::handleMessageWhenUp(msg);
    }


}

}
