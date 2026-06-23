#include "Router.h"
#include "../network/Packet.h"
#include <iostream>

using namespace std;

Router::Router(string id) : Device(id) {}

void Router::addInterface(string ifaceName, Device *link, string ip, string mac)
{
    interfaces[ifaceName] = link;
    connect(link);
    arpTable.learn(ip, mac);
    cout << "[Router " << id << "] interface " << ifaceName
         << " IP=" << ip << " MAC=" << mac << endl;
}

RoutingTable &Router::getRoutingTable() { return routingTable; }
ARPTable &Router::getARPTable() { return arpTable; }

void Router::addStaticRoute(string network, string nextHop, string iface)
{
    routingTable.addRoute(network, nextHop, iface, 1, "STATIC");
    cout << "[Router " << id << "] static route added: " << network << endl;
}

void Router::receive(Frame frame, Device *sender)
{
    cout << "[Router " << id << "] received frame on port from "
         << sender->getId() << endl;

    // route using destination field (lab: dest MAC holds target network host id)
    RouteEntry *route = routingTable.longestMatch(frame.destinationMAC);
    if (route == nullptr)
    {
        cout << "[Router " << id << "] no route to host\n";
        return;
    }

    cout << "[Router " << id << "] longest prefix match: " << route->network
         << " via " << route->nextHop << " (" << route->protocol << ")\n";

    string nextMac;
    if (!arpTable.resolve(route->nextHop, nextMac))
    {
        cout << "[Router " << id << "] ARP needed for " << route->nextHop << endl;
        return;
    }

    Frame out(frame.sourceMAC, nextMac, frame.payload, frame.sequenceNumber, frame.isACK);
    Device *outPort = interfaces[route->outInterface];
    if (outPort)
    {
        cout << "[Router " << id << "] forwarding on " << route->outInterface << endl;
        outPort->receive(out, this);
    }
}

void Router::forwardIP(Packet &pkt, Device *incoming)
{
    (void)incoming;
    RouteEntry *route = routingTable.longestMatch(pkt.destIP);
    if (!route)
    {
        cout << "[Router " << id << "] drop: no route\n";
        return;
    }
    cout << "[Network] routing " << pkt.srcIP << " -> " << pkt.destIP
         << " via " << route->network << endl;
}
