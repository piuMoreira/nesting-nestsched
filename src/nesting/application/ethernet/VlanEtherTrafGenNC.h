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

#ifndef __NESTING_VETHERTRAFGEN_H
#define __NESTING_VETHERTRAFGEN_H

#include <omnetpp.h>

#include "inet/applications/ethernet/EtherTrafGen.h"
#include <math.h>

using namespace omnetpp;
using namespace inet;

namespace nesting {

/**
 * @deprecated
 * See the NED file for a detailed description
 */
class VlanEtherTrafGenNC: public EtherTrafGen {
private:
    // Parameters from NED file
    cPar* vlanTagEnabled;
    cPar* pcp;
    cPar* dei;
    cPar* vid;

    cPar* l_gen;
    cPar* l_len;
    cPar* l_itv;

protected:
    virtual void initialize(int stage) override;
    virtual void sendBurstPackets() override;
    virtual void scheduleNextPacket(simtime_t previous);
};

} // namespace nesting

#endif /* __NESTING_VETHERTRAFGEN_H */
