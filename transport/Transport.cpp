#include "Transport.h"

void PortTable::bindPort(int port, string processName, bool wellKnown)
{
    bindings[port] = processName;
    cout << "[Transport] port " << port << " bound to " << processName;
    if (wellKnown)
        cout << " (well-known)";
    else
        cout << " (ephemeral)";
    cout << endl;
}

bool PortTable::isBound(int port) const
{
    return bindings.find(port) != bindings.end();
}

void PortTable::print() const
{
    cout << "--- Port Table ---\n";
    for (auto &p : bindings)
        cout << p.first << " -> " << p.second << "\n";
    cout << "------------------\n";
}

TransportHeader makeTransport(string data, int srcPort, int destPort, bool useTCP)
{
    TransportHeader t;
    t.srcPort = srcPort;
    t.destPort = destPort;
    t.proto = useTCP ? "TCP" : "UDP";
    t.payload = data;
    cout << "[Transport] " << t.proto << " segment: "
         << srcPort << " -> " << destPort << " data=" << data << endl;
    return t;
}
