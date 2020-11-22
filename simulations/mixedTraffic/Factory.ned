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

package nesting.simulations.mixedTraffic;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import ned.DatarateChannel;
import nesting.node.ethernet.VlanEtherSwitchPreemptable;
import nesting.node.nesting.NestingStandardHost;


channel C extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 1Gbps;
}

module Machine
{
    parameters:
        int numHosts = default(3);
        @display("bgb=450,450;i=old/cloud;");
    gates:
        inout ethg[1];
    submodules:
        s[numHosts]: VlanEtherSwitchPreemptable {
            @display("p=140,140,ri,80,80");
        }
        h[numHosts]: NestingStandardHost {
            @display("p=80,80,ri,140,140");
        }
    connections:
        // Connect switches
        for i=0..numHosts-2 {
            s[i].ethg++ <--> C <--> s[(i+1)%numHosts].ethg++;
        }
        // Connect hosts
        for i=0..numHosts-1 {
            s[i].ethg++ <--> C <--> h[i].ethg++;
        }
        // External connections
        ethg[0] <--> s[0].ethg++;
}

module ProductionCell
{
    parameters:
        @display("i=old/cloud;bgb=450,450;");
        int numMachines = default(3);
    gates:
        inout ethg[1];
    submodules:
        s[numMachines]: VlanEtherSwitchPreemptable {
            @display("p=140,140,ri,80,80");
        }
        machine[numMachines]: Machine {
            @display("p=80,80,ri,140,140");
        }
    connections:
        // Connect switches
        for i=0..numMachines-2 {
            s[i].ethg++ <--> C <--> s[(i+1)%numMachines].ethg++;
        }
        // Connect machines
        for i=0..numMachines-1 {
            machine[i].ethg[0] <--> C <--> s[i].ethg++;
            //machine[i].ethg[1] <--> C <--> s[(i+1)%numMachines].ethg++;
        }
        // External connections
        ethg[0] <--> s[0].ethg++;
}

network Factory
{
    parameters:
        int numProductionCells = default(3);
        @display("bgb=450,450;");
    submodules:

        s[numProductionCells]: VlanEtherSwitchPreemptable {
            @display("p=140,140,ri,80,80");
        }
        productionCell[numProductionCells]: ProductionCell {
            @display("p=80,80,ri,140,140");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("is=s;p=49,35");
        }
    connections:
        // Connect switches
        for i=0..numProductionCells-2 {
            s[i].ethg++ <--> C <--> s[(i+1)%numProductionCells].ethg++;
        }
        // Connect production cells
        for i=0..numProductionCells-1 {
            productionCell[i].ethg[0] <--> C <--> s[i].ethg++;
            //productionCell[i].ethg[1] <--> C <--> s[(i+1)%numProductionCells].ethg++;
        }
}
