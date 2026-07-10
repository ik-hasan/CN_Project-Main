#include "DomainAnalyzer.h"

void printDomainReport(string testName, DomainReport r)
{
    cout << "\n[" << testName << " Domain Analysis]\n";
    cout << "Broadcast domains : " << r.broadcastDomains << endl;
    cout << "Collision domains : " << r.collisionDomains << endl;
    cout << "(Switch separates collision domains per port; hub shares one domain per star)\n\n";
}

DomainReport analyzeSwitchLAN(int hostCount)
{
    DomainReport r;
    r.broadcastDomains = 1;
    r.collisionDomains = hostCount; // each switch port = separate collision domain
    return r;
}

DomainReport analyzeDualHubLAN(int hostsPerHub)
{
    (void)hostsPerHub;
    DomainReport r;
    r.broadcastDomains = 1; // switch bridges both hub LANs into one broadcast domain
    r.collisionDomains = 2; // one collision domain per hub star
    return r;
}
