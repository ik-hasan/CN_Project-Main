# ITL351 Computer Network Simulator — Specification Document

## 1. Project Goal
Develop a console-based network simulator that demonstrates the **entire protocol stack** from Physical layer to Application layer, using logs (no real packets on wire).

## 2. Language and Structure
- **Language:** C++17
- **Style:** Step-by-step classes, short comments, beginner-friendly
- **No menu** for “press 1 for Physical layer” — all tests run in sequence from `main.cpp`

## 3. Layer Implementation

| Layer | Features |
|-------|----------|
| Physical | End devices, Hub, star & point-to-point topologies, CSMA/CD (`Channel`) |
| Data Link | Frame, CRC-16, Bridge, Switch, MAC learning, GBN & Selective Repeat |
| Network | Classless IPv4, ARP, Router, static routes, longest prefix match, RIP/OSPF/BGP/EIGRP (simplified) |
| Transport | TCP/UDP segments, well-known & ephemeral ports |
| Application | HTTP and DNS (simulated messages) |

## 4. Submission Mapping

### Submission 1
- Point-to-point: two end devices
- Star: five end devices + hub (broadcast)
- Bridges and switches with MAC learning
- Error control: CRC-16-CCITT

### Submission 2 (Data Link test cases)
- **Test 1:** 1 switch + 5 hosts — CSMA/CD + GBN + domain report
- **Test 2:** 2 hubs (5 hosts each) + switch — cross-LAN traffic + domain report

### Submission 3 (Network)
- Router, CIDR IPv4, ARP, static routing, dynamic protocols, longest mask match

### Submission 4 (Transport + Application)
- Port binding, GBN at data link (reused concept for reliability)
- HTTP (TCP) and DNS (UDP) with encapsulation logs

## 5. How to Build and Run
```bat
build.bat
network_sim.exe
```

Or manually:
```bat
g++ -std=c++17 -o network_sim.exe main.cpp devices/*.cpp network/*.cpp transport/Transport.cpp application/AppLayer.cpp simulator/*.cpp
```

## 6. Test Output
Each section prints a banner and layer logs so examiners can verify requirements without a GUI.
