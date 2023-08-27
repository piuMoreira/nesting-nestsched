//
// @author: Renan Silva
//

#ifndef __FAULTY_DELAYER_H
#define __FAULTY_DELAYER_H

#include "inet/common/INETDefs.h"

namespace nesting {

/**
 * Delayer module.
 */
class FaultyDelayer : public inet::cSimpleModule
{
  public:
    FaultyDelayer() { delayRngPar = nullptr; }

  protected:
    virtual void initialize() override;
    virtual void handleMessage(inet::cMessage *msg) override;

    inet::cPar *delayRngPar;
    inet::cPar *faultProbPar;
    inet::cPar* probDistribution;

    static inet::simsignal_t delaySignal;
};

} // namespace nesting

#endif // ifndef __FAULTY_DELAYER_H

