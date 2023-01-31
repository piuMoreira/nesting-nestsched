package nesting.faulty.node;

import inet.linklayer.contract.IEthernetInterface;
import inet.networklayer.common.InterfaceTable;
import nesting.faulty.application.FaultyTrafGen;
import nesting.common.time.IClock;
import nesting.common.time.IOscillator;


module FaultyHost
{
    parameters:
        string fcsMode @enum("declared","computed") = default("declared");
        @networkNode();
        @labels(node,ethernet-node);
        @display("i=device/pc2;bgb=385,371");
        *.fcsMode = fcsMode;
        **.clockModule = default(absPath(".legacyClock"));
        **.oscillatorModule = default(absPath(".oscillator"));
        **.interfaceTableModule = default(absPath(".interfaceTable"));
    gates:
        inout ethg @labels(EtherFrame-conn);
    submodules:
        trafGenSchedApp: FaultyTrafGen {
            @display("p=138,126");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=311,211;is=s");
        }
        oscillator: <default("IdealOscillator")> like IOscillator {
            @display("p=311,40;is=s");
        }
        legacyClock: <default("LegacyClock")> like IClock {
            @display("p=311,96;is=s");
        }
        eth: <default("EthernetInterface")> like IEthernetInterface {
            parameters:
                @display("p=232,283,row,150;q=txQueue");
                encap.typename = "EtherEncap";
                qEncap.typename = "EnhancedIeee8021qEncap";
                mac.typename = "EtherMACFullDuplexPreemptable";
                mac.mtu = 1500B;
                queue.typename = "Queuing";
                queue.tsAlgorithms[*].macModule = absPath(".mac");
            	queue.gateController.macModule = absPath(".mac");
            	queue.numberOfQueues = 1;
        }
    connections:
        trafGenSchedApp.out --> eth.upperLayerIn;
        eth.upperLayerOut --> trafGenSchedApp.in;

        eth.phys <--> ethg;
}