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
#include <random>
#include <iostream>

namespace nesting {

Define_Module(FaultyDelayer);

inet::simsignal_t FaultyDelayer::delaySignal = registerSignal("delay");
inet::simtime_t dPar;


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
        std::random_device rd{}; // use to seed the rng
        std::mt19937 rng{rd()}; // rng
        std::uniform_int_distribution<int> distribution(0,delayRngPar->intValue());
        float rand = (float) distribution(rng)/1000000;
        dPar = (inet::simtime_t) rand;

        emit(inet::packetReceivedSignal, msg);

        inet::simtime_t delay(dPar);
        emit(delaySignal, dPar);
        scheduleAt(inet::simTime() + dPar, msg);
    }
}

} // namespace nesting

