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
// @author: Zoltan Bojthe
//

#include "inet/common/Simsignals.h"
#include "nesting/faulty/queue/FaultyDelayer.h"

namespace nesting {

Define_Module(FaultyDelayer);

inet::simsignal_t FaultyDelayer::delaySignal = registerSignal("delay");

void FaultyDelayer::initialize()
{
    delayPar = &par("delay");
}

void FaultyDelayer::handleMessage(inet::cMessage *msg)
{
    if (msg->isSelfMessage()) {
        emit(inet::packetSentSignal, msg);
        send(msg, "out");
    }
    else {
        emit(inet::packetReceivedSignal, msg);

        inet::simtime_t delay(*delayPar);
        emit(delaySignal, delay);
        scheduleAt(inet::simTime() + delay, msg);
    }
}

} // namespace nesting

