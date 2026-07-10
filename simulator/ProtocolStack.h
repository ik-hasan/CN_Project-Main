#ifndef PROTOCOLSTACK_H
#define PROTOCOLSTACK_H

#include <string>

class EndDevice;
class Device;

using std::string;

// deliver ACK frames waiting in AckBuffer
void deliverACKs(Device *receiver);

// send data through full stack with layer-by-layer logs
void sendFullStack(EndDevice *src, EndDevice *dest,
                   string appName, string appData,
                   int srcPort, int destPort, bool useTCP);

#endif
