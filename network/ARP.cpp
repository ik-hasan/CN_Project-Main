#include "ARP.h"

void ARPTable::learn(string ip, string mac)
{
    table[ip] = mac;
}

bool ARPTable::resolve(string ip, string &macOut)
{
    if (table.find(ip) != table.end())
    {
        macOut = table[ip];
        return true;
    }
    return false;
}

void ARPTable::print() const
{
    cout << "--- ARP Table ---\n";
    for (auto &e : table)
        cout << e.first << " -> " << e.second << "\n";
    cout << "-----------------\n";
}

string arpResolve(const string &targetIP, ARPTable &cache, const string &knownMAC)
{
    string mac;
    if (cache.resolve(targetIP, mac))
    {
        cout << "[ARP] cache hit: " << targetIP << " -> " << mac << endl;
        return mac;
    }

    // simulate ARP request and reply
    cout << "[ARP] broadcasting ARP request: Who has " << targetIP << "?\n";
    cout << "[ARP] reply: " << targetIP << " is at " << knownMAC << endl;
    cache.learn(targetIP, knownMAC);
    return knownMAC;
}
