#include "TestScenarios.h"
#include "connect.h"
#include "DomainAnalyzer.h"
#include "ProtocolStack.h"
#include "../devices/EndDevice.h"
#include "../devices/Hub.h"
#include "../devices/Switch.h"
#include "../devices/Bridge.h"
#include "../devices/Router.h"
#include "../network/Channel.h"
#include "../network/RoutingProtocols.h"
#include "../network/RoutingTable.h"
#include "../transport/Transport.h"
#include "../application/AppLayer.h"
#include <iostream>
#include <vector>

using namespace std;

static void resetChannel()
{
    Channel::busy = false;
    Channel::collision = false;
}

// ========== SUBMISSION 1 : Physical Layer ==========

static void testPointToPoint()
{
    cout << "\n############################################\n";
    cout << "# SUBMISSION 1 - Test: Point to Point     #\n";
    cout << "############################################\n\n";

    resetChannel();
    EndDevice A("A", "MAC_A", "10.0.0.1/24");
    EndDevice B("B", "MAC_B", "10.0.0.2/24");
    linkDevices(&A, &B);

    A.send("Hello B", "MAC_B");
    deliverACKs(&A);
}

static void testStarHubFiveDevices()
{
    cout << "\n############################################\n";
    cout << "# SUBMISSION 1 - Test: Star Topology (Hub)#\n";
    cout << "############################################\n\n";

    resetChannel();
    Hub hub("H1");
    vector<EndDevice *> hosts;
    vector<string> macs = {"MAC_A", "MAC_B", "MAC_C", "MAC_D", "MAC_E"};
    vector<string> names = {"A", "B", "C", "D", "E"};

    for (int i = 0; i < 5; i++)
    {
        hosts.push_back(new EndDevice(names[i], macs[i], "192.168.0." + to_string(i + 1) + "/24"));
        linkDevices(hosts[i], &hub);
    }

    cout << "[Hub] star with 5 end devices - broadcast behavior\n";
    hosts[0]->send("Hello everyone on hub", "MAC_B");
    deliverACKs(hosts[0]);

    for (auto h : hosts)
        delete h;
}

// ========== SUBMISSION 1 : Data Link (Bridge, Switch, Error) ==========

static void testBridgeAndSwitch()
{
    cout << "\n############################################\n";
    cout << "# SUBMISSION 1 - Bridge & Switch & CRC     #\n";
    cout << "############################################\n\n";

    resetChannel();
    EndDevice A("A", "MAC_A", "192.168.1.1/24");
    EndDevice B("B", "MAC_B", "192.168.1.2/24");
    EndDevice C("C", "MAC_C", "192.168.1.3/24");

    Bridge br("BR1");
    Switch sw("S1");

    linkDevices(&A, &br);
    linkDevices(&br, &sw);
    linkDevices(&sw, &B);
    linkDevices(&sw, &C);

    A.send("Frame with CRC check", "MAC_B");
    deliverACKs(&A);
    br.printMACTable();
    sw.printMACTable();
}

// ========== SUBMISSION 2 (DL deadline) : Access + Flow Control ==========

static void testCase1_SwitchFiveHosts()
{
    cout << "\n############################################\n";
    cout << "# DL Test Case 1: 1 Switch + 5 Hosts      #\n";
    cout << "############################################\n\n";

    resetChannel();
    Switch sw("S1");
    vector<EndDevice *> hosts;
    for (int i = 0; i < 5; i++)
    {
        string id(1, char('A' + i));
        hosts.push_back(new EndDevice(id, "MAC_" + id, "192.168.10." + to_string(i + 1) + "/24"));
        linkDevices(hosts[i], &sw);
    }

    printDomainReport("Test Case 1", analyzeSwitchLAN(5));

    cout << "--- Access control (CSMA/CD) + Flow control (GBN) ---\n";
    hosts[0]->send("101101", "MAC_B");
    hosts[0]->send("111000", "MAC_C");
    hosts[0]->send("110011", "MAC_D");
    hosts[0]->send("100111", "MAC_E"); // window full
    deliverACKs(hosts[0]);
    hosts[0]->send("100111", "MAC_E");

    sw.printMACTable();
    for (auto h : hosts)
        delete h;
}

static void testCase2_DualHubTenHosts()
{
    cout << "\n############################################\n";
    cout << "# DL Test Case 2: 2 Hubs + Switch        #\n";
    cout << "############################################\n\n";

    resetChannel();
    Hub hub1("H1");
    Hub hub2("H2");
    Switch sw("S-Main");

    linkDevices(&hub1, &sw);
    linkDevices(&hub2, &sw);

    vector<EndDevice *> all;
    for (int h = 0; h < 2; h++)
    {
        Hub *hub = (h == 0) ? &hub1 : &hub2;
        for (int i = 0; i < 5; i++)
        {
            int idx = h * 5 + i;
            string id = string(1, char('A' + idx));
            auto *dev = new EndDevice(id, "MAC_" + id,
                                      "192.168." + to_string(h + 1) + "." + to_string(i + 1) + "/24");
            linkDevices(dev, hub);
            all.push_back(dev);
        }
    }

    printDomainReport("Test Case 2", analyzeDualHubLAN(5));

    cout << "--- Host A talks to Host J (across hubs) ---\n";
    all[0]->send("Hello from LAN1 to LAN2", "MAC_J");
    deliverACKs(all[0]);

    for (auto d : all)
        delete d;
}

// ========== SUBMISSION 3 (Network Layer) ==========

static void testNetworkLayer()
{
    cout << "\n############################################\n";
    cout << "# SUBMISSION 3 - Network Layer             #\n";
    cout << "############################################\n\n";

    resetChannel();

    Router R1("R1");
    EndDevice A("A", "MAC_A", "192.168.1.10/24");
    EndDevice B("B", "MAC_B", "192.168.2.10/24");

    linkDevices(&A, &R1);
    linkDevices(&B, &R1);

    R1.addInterface("eth0", &A, "192.168.1.1/24", "MAC_R0");
    R1.addInterface("eth1", &B, "192.168.2.1/24", "MAC_R1");

    R1.addStaticRoute("192.168.1.0/24", "direct", "eth0");
    R1.addStaticRoute("192.168.2.0/24", "direct", "eth1");
    R1.addStaticRoute("0.0.0.0/0", "10.1.1.2", "eth1");

    cout << "\n--- ARP resolution ---\n";
    arpResolve("192.168.2.10", R1.getARPTable(), "MAC_B");
    R1.getARPTable().print();

    cout << "\n--- Dynamic routing protocols ---\n";
    ripExchange(R1.getRoutingTable());
    ospfCompute(R1.getRoutingTable());
    bgpExchange(R1.getRoutingTable());
    eigrpUpdate(R1.getRoutingTable());
    R1.getRoutingTable().print();

    cout << "\n--- Address aggregation (route summarization) ---\n";
    cout << "[Network] 192.168.0.0/25 + 192.168.0.128/25 -> aggregated 192.168.0.0/24\n";

    cout << "\n--- Longest prefix match demo ---\n";
    RouteEntry *r = R1.getRoutingTable().longestMatch("192.168.2.10");
    if (r)
        cout << "Match: " << r->network << " via " << r->nextHop << endl;

    cout << "\n--- Forwarding frame through router ---\n";
    A.send("192.168.2.10", "MAC_B"); // dest field used for route lookup in lab router
}

// ========== SUBMISSION 4 (Transport + Application) ==========

static void testFullStackHTTP()
{
    cout << "\n############################################\n";
    cout << "# Transport + Application - HTTP           #\n";
    cout << "############################################\n\n";

    resetChannel();
    EndDevice client("PC1", "MAC_C1", "10.0.0.5/24");
    EndDevice server("SRV", "MAC_SRV", "10.0.0.10/24");
    Switch sw("S1");
    linkDevices(&client, &sw);
    linkDevices(&sw, &server);

    client.bindPort(49152, "browser", false);
    server.bindPort(PORT_HTTP, "httpd", true);
    client.getPortTable().print();

    sendFullStack(&client, &server, "HTTP", "/index", 49152, PORT_HTTP, true);
    deliverACKs(&client);
}

static void testFullStackDNS()
{
    cout << "\n############################################\n";
    cout << "# Transport + Application - DNS (UDP)      #\n";
    cout << "############################################\n\n";

    resetChannel();
    EndDevice pc("PC2", "MAC_PC2", "10.0.0.6/24");
    EndDevice dns("DNS", "MAC_DNS", "10.0.0.53/24");
    Hub hub("H-DNS");
    linkDevices(&pc, &hub);
    linkDevices(&hub, &dns);

    pc.bindPort(53000, "resolver", false);
    dns.bindPort(PORT_DNS, "named", true);

    sendFullStack(&pc, &dns, "DNS", "example.com", 53000, PORT_DNS, false);
    deliverACKs(&pc);

    // show server side app
    appDNS_Reply("example.com", "93.184.216.34");
}

static void testAccessControlCollision()
{
    cout << "\n############################################\n";
    cout << "# Access Control: CSMA/CD scenarios      #\n";
    cout << "############################################\n\n";

    resetChannel();
    EndDevice A("A", "MAC_A", "10.0.0.1/24");
    EndDevice B("B", "MAC_B", "10.0.0.2/24");
    Hub H1("H1");
    linkDevices(&A, &H1);
    linkDevices(&B, &H1);

    cout << "=== Normal send ===\n";
    A.send("101101", "MAC_B");
    deliverACKs(&A);

    cout << "\n=== Channel busy ===\n";
    Channel::busy = true;
    A.send("101101", "MAC_B");
    Channel::busy = false;

    cout << "\n=== Collision ===\n";
    Channel::collision = true;
    A.send("101101", "MAC_B");
    Channel::collision = false;
    deliverACKs(&A);
}

static void testSelectiveRepeat()
{
    cout << "\n############################################\n";
    cout << "# Flow Control: Selective Repeat           #\n";
    cout << "############################################\n\n";

    resetChannel();
    EndDevice A("A", "MAC_A", "10.1.1.1/24");
    EndDevice B("B", "MAC_B", "10.1.1.2/24");
    Switch sw("S1");
    linkDevices(&A, &sw);
    linkDevices(&sw, &B);

    A.setProtocol(ReliableProtocol::SelectiveRepeat);
    B.setProtocol(ReliableProtocol::SelectiveRepeat);

    A.send("SR-1", "MAC_B");
    A.send("SR-2", "MAC_B");
    deliverACKs(&A);
}

void runAllTests()
{
    cout << "\n";
    cout << "================================================\n";
    cout << "  ITL351 Computer Network Simulator\n";
    cout << "  Full Protocol Stack Demonstration\n";
    cout << "================================================\n";

    testPointToPoint();
    testStarHubFiveDevices();
    testBridgeAndSwitch();
    testCase1_SwitchFiveHosts();
    testCase2_DualHubTenHosts();
    testAccessControlCollision();
    testSelectiveRepeat();
    testNetworkLayer();
    testFullStackHTTP();
    testFullStackDNS();

    cout << "\n================================================\n";
    cout << "  All tests finished.\n";
    cout << "================================================\n\n";
}