#ifndef PACKET_H
#define PACKET_H

#include <string>

using namespace std;

// one PDU that travels through all layers (logged at each step)
struct Packet
{
    // application layer
    string appName;
    string appData;

    // transport layer
    int srcPort;
    int destPort;
    string transportProto; // TCP or UDP

    // network layer
    string srcIP;
    string destIP;
    string ttl;
};

#endif