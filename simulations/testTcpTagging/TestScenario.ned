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

package nesting.simulations.testTcpTagging;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth1G;
import nesting.node.ethernet.VlanEtherSwitchPreemptable;
import nesting.node.nesting.NestingStandardHost;


network TestScenario
{
    @display("bgb=366,178");
    submodules:
        server: NestingStandardHost {
            @display("p=65,110");
        }
        client: NestingStandardHost {
            @display("p=293,110");
        }
        s1: VlanEtherSwitchPreemptable {
            @display("p=171,109");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("is=s;p=57,33");
        }
    connections:
       	server.ethg++ <--> Eth1G <--> s1.ethg++;
        s1.ethg++ <--> Eth1G <--> client.ethg++;
}
