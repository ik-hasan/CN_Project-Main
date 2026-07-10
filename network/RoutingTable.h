#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct RouteEntry
{
    string network; // e.g. 192.168.1.0/24
    string nextHop; // IP or "direct"
    string outInterface;
    int metric;
    string protocol; // STATIC, RIP, OSPF, BGP, EIGRP
};

// routing table with longest prefix match
class RoutingTable
{
public:
    vector<RouteEntry> routes;

    void addRoute(string network, string nextHop, string iface, int metric, string proto);
    RouteEntry *longestMatch(string destIP);
    void print() const;
};

#endif
