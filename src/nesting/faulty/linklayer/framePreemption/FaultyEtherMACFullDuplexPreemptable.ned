package nesting.faulty.linklayer.framePreemption;

import inet.linklayer.contract.IEtherMac;
import nesting.linklayer.framePreemption.EtherMACFullDuplexPreemptable;


//
// This module is a new version for the EtherMACFullDuplexPreemptable module adding a drop fault using the Bernoulli Distribution.
//
simple FaultyEtherMACFullDuplexPreemptable extends EtherMACFullDuplexPreemptable like IEtherMac
{
    parameters:
        @class(FaultyEtherMACFullDuplexPreemptable);
        double dropProbability = default(0.1); // probability to drop a frame
}

