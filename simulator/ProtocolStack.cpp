#include "ProtocolStack.h"
#include "../devices/EndDevice.h"
#include "../network/Packet.h"
#include "../network/ARP.h"
#include "../transport/Transport.h"
#include "../application/AppLayer.h"
#include "../network/AckBuffer.h"
#include <iostream>

using namespace std;

void deliverACKs(Device *receiver)
{
    while (!AckBuffer::buffer.empty())
    {
        Frame ack = AckBuffer::buffer.front();
        AckBuffer::buffer.pop();
        receiver->receive(ack, receiver);
    }
}

void sendFullStack(EndDevice *src, EndDevice *dest,
                   string appName, string appData,
                   int srcPort, int destPort, bool useTCP)
{
    cout << "\n========== FULL PROTOCOL STACK ==========\n";

    Packet pkt;
    pkt.appName = appName;
    pkt.appData = appData;

    // application layer
    cout << "\n--- Layer 7 Application ---\n";
    if (appName == "HTTP")
        pkt.appData = appHTTP_Client(dest->getIP(), "/index");
    else if (appName == "DNS")
        pkt.appData = appDNS_Query(appData);

    // transport layer
    cout << "\n--- Layer 4 Transport ---\n";
    TransportHeader th = makeTransport(pkt.appData, srcPort, destPort, useTCP);
    pkt.srcPort = th.srcPort;
    pkt.destPort = th.destPort;
    pkt.transportProto = th.proto;

    // network layer
    cout << "\n--- Layer 3 Network ---\n";
    pkt.srcIP = src->getIP();
    pkt.destIP = dest->getIP();
    pkt.ttl = "64";
    cout << "[Network] IPv4 packet " << pkt.srcIP << " -> " << pkt.destIP
         << " TTL=" << pkt.ttl << " proto=" << pkt.transportProto << endl;

    // ARP before building frame
    string destMAC = arpResolve(pkt.destIP, src->getARPTable(), dest->getMAC());

    // data link + physical (send frame through connections)
    cout << "\n--- Layer 2 Data Link ---\n";
    cout << "[Data Link] building Ethernet frame\n";

    cout << "\n--- Layer 1 Physical ---\n";
    cout << "[Physical] transmitting bits on medium (CSMA/CD)\n";

    src->send(th.payload, destMAC);
    deliverACKs(src);
}
