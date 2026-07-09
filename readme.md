# ITL351 Computer Network Simulator

Semester project — full protocol stack simulator (C++).

## Project structure

```
Computer-Network-Simulator/
├── main.cpp
├── build.bat
├── SPECIFICATION.md
├── devices/       Device, EndDevice, Hub, Switch, Bridge, Router
├── network/       Frame, Channel, CRC, IPv4, ARP, RoutingTable, RoutingProtocols
├── transport/     Ports, TCP/UDP segments
├── application/   HTTP, DNS
└── simulator/     Tests, domain analysis, full stack send
```

## Build and run (Windows)

```bat
cd Computer-Network-Simulator
.\build.bat
network_sim.exe
```

## What runs

`main.cpp` calls `runAllTests()` which runs **all submission test cases** in order:

1. Point-to-point
2. Star topology (5 hosts + hub)
3. Bridge + Switch + CRC
4. DL Test Case 1 (switch + 5 hosts + domains)
5. DL Test Case 2 (2 hubs + switch + 10 hosts + domains)
6. CSMA/CD access control
7. Selective Repeat flow control
8. Network layer (router, ARP, static + RIP/OSPF/BGP/EIGRP)
9. Full stack HTTP (TCP)
10. Full stack DNS (UDP)

## Design notes

- **Frames** carry data at Layer 2; **IPv4/ports** logged at upper layers.
- **ACKs** use `AckBuffer` and `deliverACKs()` like earlier steps.
- **Routing protocols** are simplified educational simulations that update the routing table and print logs.

See `SPECIFICATION.md` for mapping to assignment deliverables.
