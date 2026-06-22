#ifndef ENDDEVICE_H
#define ENDDEVICE_H

#include "Device.h"
#include "../network/ARP.h"
#include "../transport/Transport.h"

enum class ReliableProtocol
{
    GBN,
    SelectiveRepeat
};

class EndDevice : public Device
{

private:
    string macAddress;
    string ipAddress; // classless IPv4 e.g. 192.168.1.10/24
    ReliableProtocol protocol;
    int nextSeq;
    int base;
    int windowSize;

    ARPTable arpTable;
    PortTable portTable;

    // GBN receiver
    int expectedSeq;

    // Selective Repeat receiver
    int rcv_base;
    vector<bool> sr_recvFilled;
    vector<string> sr_recvPayload;

    // Selective Repeat sender: per-sequence ACK received
    vector<bool> sr_acked;

public:
    static const int SEQ_MOD = 8;

    EndDevice(string id, string mac, string ip = "0.0.0.0/0",
              ReliableProtocol p = ReliableProtocol::GBN);

    string getMAC();
    string getIP();
    void setIP(string cidr);

    ARPTable &getARPTable();
    PortTable &getPortTable();
    void bindPort(int port, string processName, bool wellKnown);

    ReliableProtocol getProtocol() const { return protocol; }

    void setProtocol(ReliableProtocol p);

    void setWindowSize(int w);

    // data link send (frame)
    void send(string data, string destMAC);

    void receive(Frame frame, Device *sender) override;
};

#endif
