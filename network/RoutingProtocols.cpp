#include "RoutingProtocols.h"
#include <iostream>

using namespace std;

void ripExchange(RoutingTable &table)
{
    cout << "\n[RIP] sending full routing update (distance-vector)\n";
    table.addRoute("10.0.0.0/8", "10.1.1.2", "eth0", 2, "RIP");
    table.addRoute("172.16.0.0/16", "10.1.1.2", "eth0", 3, "RIP");
    cout << "[RIP] update received and merged\n";
}

void ospfCompute(RoutingTable &table)
{
    cout << "\n[OSPF] running link-state SPF on area 0\n";
    table.addRoute("192.168.2.0/24", "10.1.1.2", "eth1", 10, "OSPF");
    cout << "[OSPF] shortest path tree installed\n";
}

void bgpExchange(RoutingTable &table)
{
    cout << "\n[BGP] exchanging path vectors with neighbor AS 65001\n";
    table.addRoute("203.0.113.0/24", "203.0.113.1", "eth2", 0, "BGP");
    cout << "[BGP] route advertised with AS-PATH\n";
}

void eigrpUpdate(RoutingTable &table)
{
    cout << "\n[EIGRP] sending composite metric update\n";
    table.addRoute("192.168.3.0/24", "10.1.1.3", "eth1", 3072, "EIGRP");
    cout << "[EIGRP] feasible successor stored\n";
}
