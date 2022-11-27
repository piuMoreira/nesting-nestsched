//
// Copyright (C) 2012 OpenSim Ltd
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
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

}

void FaultyDelayer::handleMessage(inet::cMessage *msg)
{
    if (msg->isSelfMessage()) {
        emit(inet::packetSentSignal, msg);
        send(msg, "out");
    }
    else {
        int isFaulty = distr.uniformDistribution(100);
        if (isFaulty == 1) {
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

