#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// well-known ports used in lab
const int PORT_DNS = 53;
const int PORT_HTTP = 80;

// bind a port to a process name on a host
class PortTable
{
public:
    unordered_map<int, string> bindings;

    void bindPort(int port, string processName, bool wellKnown);
    bool isBound(int port) const;
    void print() const;
};

// build transport header (TCP segment or UDP datagram)
struct TransportHeader
{
    int srcPort;
    int destPort;
    string proto;
    string payload;
};

TransportHeader makeTransport(string data, int srcPort, int destPort, bool useTCP);

#endif
