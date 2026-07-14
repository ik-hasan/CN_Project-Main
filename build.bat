@echo off
echo Compiling Network Simulator...
g++ -std=c++17 -o network_sim.exe main.cpp ^
  devices/Device.cpp devices/EndDevice.cpp devices/Hub.cpp devices/Switch.cpp devices/Bridge.cpp devices/Router.cpp ^
  network/Crc.cpp network/Channel.cpp network/AckBuffer.cpp network/IPv4.cpp network/ARP.cpp network/RoutingTable.cpp network/RoutingProtocols.cpp ^
  transport/Transport.cpp application/AppLayer.cpp ^
  simulator/connect.cpp simulator/DomainAnalyzer.cpp simulator/ProtocolStack.cpp simulator/TestScenarios.cpp
if %errorlevel%==0 (
  echo Build OK. Run: network_sim.exe
) else (
  echo Build FAILED.
)
