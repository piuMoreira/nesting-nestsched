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

package nesting.application.ethernet;

import inet.applications.ethernet.EtherTrafGen;


//
//
// Traffic generator application for VLAN tagged ethernet frames.
//
// This module uses a Poisson Distribution to generate the traffic.
//
//
simple VlanEtherTrafGenNC extends EtherTrafGen
{
    parameters:
        @class(VlanEtherTrafGenNC);
        bool vlanTagEnabled = default(true);	// When true, the application's packets will be VLAN encapsulated by the VLANEncap module
        int pcp = default(0);					// PCP (Priority Code Point) value that is added to the packets control information
        bool dei = default(false);				// DEI (Drop Eligible Indicator) that is added to the packets control information
        int vid = default(1);					// VID (VLAN Identifier) value that is added to the packets control information
        bool verbose = default(false);
        
        int l_gen = default(3);					//Lambda value for the number of packets sent per burst
        int l_len = default(500);				//Lambda value for the length of the packet
        int l_itv = default(10);				//Lambda value for the periodicity of packet burst
}
