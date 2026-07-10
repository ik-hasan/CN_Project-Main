#ifndef ROUTINGPROTOCOLS_H
#define ROUTINGPROTOCOLS_H

#include "RoutingTable.h"

// simplified educational versions of dynamic routing protocols
// they only update the routing table and print logs

void ripExchange(RoutingTable &table);
void ospfCompute(RoutingTable &table);
void bgpExchange(RoutingTable &table);
void eigrpUpdate(RoutingTable &table);

#endif
