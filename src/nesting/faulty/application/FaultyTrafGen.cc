#include "nesting/faulty/application/FaultyTrafGen.h"

#include "nesting/faulty/common/distributions/Distributions.h"

namespace nesting {

Define_Module(FaultyTrafGen);

Distributions distrib;

FaultyTrafGen::~FaultyTrafGen() {

}

void FaultyTrafGen::initialize(int stage) {
    VlanEtherTrafGenSched::initialize(stage);

    dropAtStartProbPar = &par("dropAtStartProb");
    dropAtEndProbPar = &par("dropAtEndProb");
    probDistribution = &par("probDistribution");

}

void FaultyTrafGen::handleMessage(cMessage *msg) {
    int isFaulty = distrib.distributionRun(probDistribution->intValue(), 100);
    if (!(msg->isSelfMessage()) && isFaulty <= dropAtEndProbPar->intValue() && dropAtEndProbPar->intValue() != 0) {
        return;
    } else {
        VlanEtherTrafGenSched::handleMessage(msg);
    }

}

void FaultyTrafGen::sendPacket(uint64_t scheduleIndexTx) {
    int isFaulty = distrib.distributionRun(probDistribution->intValue(), 100);
    if (isFaulty <= dropAtStartProbPar->intValue() && dropAtStartProbPar->intValue() != 0) {
        VlanEtherTrafGenSched::maxNumberOfPackets--;
    } else {
        VlanEtherTrafGenSched::sendPacket(scheduleIndexTx);
    }
}



}
