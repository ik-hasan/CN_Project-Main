#include "Bridge.h"

Bridge::Bridge(string id) : Device(id) {}

void Bridge::receive(Frame frame, Device *sender)
{
    cout << "[Bridge " << id << "] received frame\n";

    //learning mac
    macTable[frame.sourceMAC] = sender;
    cout << "[Bridge " << id << "] learned " << frame.sourceMAC << endl;

    if (macTable.find(frame.destinationMAC) != macTable.end())
    {
        Device *target = macTable[frame.destinationMAC];
        cout << "[Bridge " << id << "] forwarding to " << target->getId() << endl;
        target->receive(frame, this);
    }
    else
    {
        cout << "[Bridge " << id << "] unknown dest -> flooding\n";
        for (auto d : connections)
        {
            if (d != sender)
                d->receive(frame, this);
        }
    }
}

void Bridge::printMACTable()
{
    cout << "\n[Bridge " << id << "] MAC Table\n";
    for (auto &e : macTable)
        cout << e.first << " -> " << e.second->getId() << "\n";
    cout << endl;
}
