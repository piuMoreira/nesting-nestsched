package nesting.simulations.examples.nestSched;

import ned.DatarateChannel;
import nesting.node.ethernet.VlanEtherSwitchPreemptable;
import nesting.node.ethernet.VlanEtherHostNC;

network nestSched
{
    @display("bgb=500,400,white");
    types:
        channel C extends DatarateChannel
        {
            delay = 0ns;
            datarate = 1Gbps;
        }

    submodules:
        dev0: NestSchedDev {
            @display("p=244,71");
        }
        switch0: VlanEtherSwitchPreemptable {
            parameters:
                @display("p=244,127");
            gates:
                ethg[2];
        }



        dev1: NestSchedDev {
            @display("p=71,110");
        }
        dev2: NestSchedDev {
            @display("p=71,175");
        }
        dev3: NestSchedDev {
            @display("p=71,233");
        }
        ncdev1: VlanEtherHostNC {
            @display("p=71,50");
        }
        ncdev2: VlanEtherHostNC {
            @display("p=71,303");
        }
        switch1: VlanEtherSwitchPreemptable {
            parameters:
                @display("p=153,185");
            gates:
                ethg[6];
        }



        dev4: NestSchedDev {
            @display("p=192,264");
        }
        dev5: NestSchedDev {
            @display("p=244,264");
        }
        dev6: NestSchedDev {
            @display("p=296,264");
        }
        ncdev3: VlanEtherHostNC {
            @display("p=198,354");
        }
        ncdev4: VlanEtherHostNC {
            @display("p=283,354");
        }
        switch2: VlanEtherSwitchPreemptable {
            parameters:
                @display("p=244,185");
            gates:
                ethg[8];
        }



        dev7: NestSchedDev {
            @display("p=400,110");
        }
        dev8: NestSchedDev {
            @display("p=400,175");
        }
        dev9: NestSchedDev {
            @display("p=400,233");
        }
        ncdev5: VlanEtherHostNC {
            @display("p=400,50");
        }
        ncdev6: VlanEtherHostNC {
            @display("p=400,303");
        }
        switch3: VlanEtherSwitchPreemptable {
            parameters:
                @display("p=335,185");
            gates:
                ethg[6];
        }



    connections:
        switch0.ethg[0] <--> C <--> switch2.ethg[0];
        switch0.ethg[1] <--> C <--> dev0.ethg;


        switch1.ethg[0] <--> C <--> switch2.ethg[1];
        switch1.ethg[1] <--> C <--> dev1.ethg;
        switch1.ethg[2] <--> C <--> dev2.ethg;
        switch1.ethg[3] <--> C <--> dev3.ethg;
        switch1.ethg[4] <--> C <--> ncdev1.ethg;
        switch1.ethg[5] <--> C <--> ncdev2.ethg;


        switch2.ethg[2] <--> C <--> switch3.ethg[0];
        switch2.ethg[3] <--> C <--> dev4.ethg;
        switch2.ethg[4] <--> C <--> dev5.ethg;
        switch2.ethg[5] <--> C <--> dev6.ethg;
        switch2.ethg[6] <--> C <--> ncdev3.ethg;
        switch2.ethg[7] <--> C <--> ncdev4.ethg;


        switch3.ethg[1] <--> C <--> dev7.ethg;
        switch3.ethg[2] <--> C <--> dev8.ethg;
        switch3.ethg[3] <--> C <--> dev9.ethg;
        switch3.ethg[4] <--> C <--> ncdev5.ethg;
        switch3.ethg[5] <--> C <--> ncdev6.ethg;


}