package nesting.faulty.linklayer.framePreemption;


import nesting.linklayer.framePreemption.EtherMACFullDuplexPreemptable;

simple FaultyMACFullDuplexPreemptable extends EtherMACFullDuplexPreemptable
{
    parameters:
        @class(FaultyMACFullDuplexPreemptable);
    	int dropProbability = default(0);  //Probability to drop a packet
    	int probDistribution = default(1);
}
