#include "RoutingTable.h"
#include "IPv4.h"

void RoutingTable::addRoute(string network, string nextHop, string iface, int metric, string proto)
{
    RouteEntry r;
    r.network = network;
    r.nextHop = nextHop;
    r.outInterface = iface;
    r.metric = metric;
    r.protocol = proto;
    routes.push_back(r);
}

RouteEntry *RoutingTable::longestMatch(string destIP)
{
    RouteEntry *best = nullptr;
    int bestLen = -1;

    for (auto &r : routes)
    {
        string netIP;
        int prefix = 0;
        IPv4::parseCIDR(r.network, netIP, prefix);

        IPv4 dest(destIP + "/32");
        IPv4 net(r.network);

        // simple match for lab networks (/24)
        if (dest.sameNetwork(net))
        {
            if (prefix > bestLen)
            {
                bestLen = prefix;
                best = &r;
            }
        }
    }
    return best;
}

void RoutingTable::print() const
{
    cout << "--- Routing Table ---\n";
    for (auto &r : routes)
    {
        cout << r.network << " via " << r.nextHop
             << " [" << r.protocol << "] metric=" << r.metric << endl;
    }
    cout << "---------------------\n";
}
