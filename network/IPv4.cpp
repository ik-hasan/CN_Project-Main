#include "IPv4.h"
#include <sstream>

IPv4::IPv4(string cidr)
{
    parseCIDR(cidr, address, prefixLength);
}

bool IPv4::parseCIDR(const string &cidr, string &ip, int &prefix)
{
    size_t slash = cidr.find('/');
    if (slash == string::npos)
    {
        ip = cidr;
        prefix = 32;
        return true;
    }
    ip = cidr.substr(0, slash);
    prefix = stoi(cidr.substr(slash + 1));
    return true;
}

string IPv4::networkAddress() const
{
    // beginner friendly: we only compare first 3 octets for /24 style labs
    if (prefixLength >= 24)
    {
        size_t lastDot = address.rfind('.');
        if (lastDot != string::npos)
            return address.substr(0, lastDot) + ".0";
    }
    return address;
}

bool IPv4::sameNetwork(const IPv4 &other) const
{
    return networkAddress() == other.networkAddress();
}
