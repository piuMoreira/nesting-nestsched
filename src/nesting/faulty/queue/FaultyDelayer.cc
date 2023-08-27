//
// @author: Renan Silva
//

#include "inet/common/Simsignals.h"
#include "nesting/faulty/queue/FaultyDelayer.h"
#include "nesting/faulty/common/distributions/Distributions.h"

namespace nesting {

Define_Module(FaultyDelayer);

inet::simsignal_t FaultyDelayer::delaySignal = registerSignal("delay");
inet::simtime_t dPar;
Distributions distr;

void FaultyDelayer::initialize()
{
    delayRngPar = &par("delayRng");
    faultProbPar = &par("faultProb");
    probDistribution = &par("probDistribution");

}

void FaultyDelayer::handleMessage(inet::cMessage *msg)
{
    if (msg->isSelfMessage()) {
        emit(inet::packetSentSignal, msg);
        send(msg, "out");
    }
    else {
        int isFaulty = distr.distributionRun(probDistribution->intValue(), 100);
        if (isFaulty <= faultProbPar->intValue() && faultProbPar->intValue() != 0) {
            int x = distr.uniformDistribution(delayRngPar->intValue());
            float rand = (float) x/1000000;
            dPar = (inet::simtime_t) rand;
            emit(inet::packetReceivedSignal, msg);

            inet::simtime_t delay(dPar);
            emit(delaySignal, dPar);
            scheduleAt(inet::simTime() + dPar, msg);
        } else {
            dPar = 0;
            emit(inet::packetReceivedSignal, msg);

            inet::simtime_t delay(dPar);
            emit(delaySignal, dPar);
            scheduleAt(inet::simTime() + dPar, msg);
        }
    }
}

} // namespace nesting

