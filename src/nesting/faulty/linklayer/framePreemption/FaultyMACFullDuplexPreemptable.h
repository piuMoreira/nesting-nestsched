#ifndef _FAULTY_MACFULLDUPLEXPREEMPTABLE_H
#define _FAULTY_MACFULLDUPLEXPREEMPTABLE_H

#include "nesting/linklayer/framePreemption/EtherMACFullDuplexPreemptable.h"

using namespace inet;

namespace nesting {

class FaultyMACFullDuplexPreemptable: public EtherMACFullDuplexPreemptable{

protected:
    inet::cPar *dropProbabilityPar;
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;

public:
    ~FaultyMACFullDuplexPreemptable();

};

}

#endif
