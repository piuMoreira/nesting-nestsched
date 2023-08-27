// @author Renan Silva

package nesting.faulty.queue;

import inet.common.IHook;

//
// Faulty Delayer module.
//
simple FaultyDelayer like IHook
{
    parameters:
        int delayRng = default(500);				//The max value the delay can have in us
        int faultProb = default (0);           		//Probability to a fault occur in the delayer
        int probDistribution = default(1);
        @display("i=block/timer");
        @signal[packetReceived](type=cMessage);
        @signal[packetSent](type=cMessage);
        @signal[delay](type=simtime_t);     // delay in seconds, emitted at packet receiving
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[packetSent](title="packets sent"; source=packetSent; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[delay](title="packet delay"; source=delay; record=vector,stats; interpolationmode=none);
    gates:
        input in;
        output out;
}

