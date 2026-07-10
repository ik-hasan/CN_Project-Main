#ifndef IPV4_H
#define IPV4_H

#include <string>
#include <vector>

using namespace std;

// simple classless IPv4 helper (CIDR notation like 192.168.1.10/24)
class IPv4
{
public:
    string address;
    int prefixLength;

    IPv4(string cidr = "0.0.0.0/0");

    string networkAddress() const;
    bool sameNetwork(const IPv4 &other) const;
    static bool parseCIDR(const string &cidr, string &ip, int &prefix);
};

#endif
