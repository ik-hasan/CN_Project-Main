#ifndef DOMAINANALYZER_H
#define DOMAINANALYZER_H

#include <iostream>

using namespace std;

// counts broadcast and collision domains for required test cases
struct DomainReport
{
    int broadcastDomains;
    int collisionDomains;
};

void printDomainReport(string testName, DomainReport r);

// Test 1: 1 switch + 5 end devices
DomainReport analyzeSwitchLAN(int hostCount);

// Test 2: 2 hubs (5 hosts each) connected by 1 switch
DomainReport analyzeDualHubLAN(int hostsPerHub);

#endif
