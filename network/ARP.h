#ifndef ARP_H
#define ARP_H

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// ARP cache: IP -> MAC
class ARPTable
{
public:
    unordered_map<string, string> table;

    void learn(string ip, string mac);
    bool resolve(string ip, string &macOut);
    void print() const;
};

// simulate ARP request/reply on a LAN
string arpResolve(const string &targetIP, ARPTable &cache, const string &knownMAC);

#endif
