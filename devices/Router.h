#ifndef ROUTER_H
#define ROUTER_H

#include "Device.h"
#include "../network/RoutingTable.h"
#include "../network/ARP.h"
#include "../network/Packet.h"
#include <unordered_map>

class Router : public Device
{
private:
    RoutingTable routingTable;
    ARPTable arpTable;

    // interface name -> connected device
    unordered_map<string, Device *> interfaces;

public:
    Router(string id);

    void addInterface(string ifaceName, Device *link, string ip, string mac);
    RoutingTable &getRoutingTable();
    ARPTable &getARPTable();

    void addStaticRoute(string network, string nextHop, string iface);
    void receive(Frame frame, Device *sender) override;
    void forwardIP(Packet &pkt, Device *incoming);
};

#endif
