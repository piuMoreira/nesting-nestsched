#ifndef _FAULTY_TRAFGEN_H
#define _FAULTY_TRAFGEN_H

#include "nesting/application/ethernet/VlanEtherTrafGenSched.h"

using namespace omnetpp;
using namespace inet;

namespace nesting {

class FaultyTrafGen: public VlanEtherTrafGenSched {

protected:
    inet::cPar* dropAtStartProbPar;
    inet::cPar* dropAtEndProbPar;
    inet::cPar* probDistribution;

    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendPacket(uint64_t scheduleIndexTx) override;

public:
    ~FaultyTrafGen();

};


}

#endif
