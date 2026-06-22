#include "EndDevice.h"
#include "../network/Channel.h"
#include "../network/AckBuffer.h"
#include "../network/Crc.h"
#include "../application/AppLayer.h"
#include <cstdlib>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

namespace
{

int mod(int n, int m)
{
    return ((n % m) + m) % m;
}

bool inRecvWindow(int seq, int winBase, int winSize, int modN)
{
    for (int i = 0; i < winSize; ++i)
    {
        if (mod(winBase + i, modN) == mod(seq, modN))
            return true;
    }
    return false;
}

bool seqInUnacked(int ack, int base, int nextSeq, int modN)
{
    int n = mod(nextSeq - base, modN);
    for (int i = 0; i < n; ++i)
    {
        if (mod(base + i, modN) == mod(ack, modN))
            return true;
    }
    return false;
}

} // namespace

EndDevice::EndDevice(string id, string mac, string ip, ReliableProtocol p) : Device(id)
{
    macAddress = mac;
    ipAddress = ip;
    protocol = p;
    nextSeq = 0;
    base = 0;
    windowSize = 3;
    expectedSeq = 0;
    rcv_base = 0;
    sr_recvFilled.assign(SEQ_MOD, false);
    sr_recvPayload.assign(SEQ_MOD, "");
    sr_acked.assign(SEQ_MOD, false);
}

void EndDevice::setProtocol(ReliableProtocol p)
{
    protocol = p;
    nextSeq = 0;
    base = 0;
    expectedSeq = 0;
    rcv_base = 0;
    fill(sr_recvFilled.begin(), sr_recvFilled.end(), false);
    sr_recvPayload.assign(SEQ_MOD, "");
    fill(sr_acked.begin(), sr_acked.end(), false);
}

void EndDevice::setWindowSize(int w)
{
    if (w > 0 && w * 2 <= SEQ_MOD)
        windowSize = w;
}

string EndDevice::getMAC() { return macAddress; }

string EndDevice::getIP() { return ipAddress; }

void EndDevice::setIP(string cidr) { ipAddress = cidr; }

ARPTable &EndDevice::getARPTable() { return arpTable; }

PortTable &EndDevice::getPortTable() { return portTable; }

void EndDevice::bindPort(int port, string processName, bool wellKnown)
{
    portTable.bindPort(port, processName, wellKnown);
}

void EndDevice::send(string data, string destMAC)
{
    int pending = mod(nextSeq - base, SEQ_MOD);
    if (pending >= windowSize)
    {
        cout << "[" << id << "] window full, waiting for ACK\n";
        return;
    }

    int attempts = 0;
    while (attempts < 3)
    {
        if (Channel::busy)
        {
            cout << "[" << id << "] channel busy, waiting...\n";
            return;
        }
        Channel::busy = true;

        int seq = nextSeq;
        Frame frame(macAddress, destMAC, data, seq, false);

        cout << "[" << id << "] sending frame\n";
        cout << "SEQ=" << frame.sequenceNumber
             << " SRC=" << frame.sourceMAC
             << " DEST=" << frame.destinationMAC
             << " DATA=" << frame.payload
             << " CRC=" << frame.crc << endl;

        if (Channel::collision)
        {
            cout << "[" << id << "] collision detected\n";
            Channel::busy = false;
            int delay = rand() % 3 + 1;
            cout << "[" << id << "] backing off for " << delay << " seconds\n";
#ifdef _WIN32
            Sleep(delay * 1000);
#else
            sleep(delay);
#endif
            attempts++;
            continue;
        }

        for (auto device : connections)
        {
            device->receive(frame, this);
        }
        Channel::busy = false;
        nextSeq = mod(nextSeq + 1, SEQ_MOD);
        return;
    }
    cout << "[" << id << "] transmission failed after retries\n";
}

void EndDevice::receive(Frame frame, Device *sender)
{
    if (frame.destinationMAC != macAddress)
    {
        cout << "[" << id << "] Silently ignoring frame\n";
        return;
    }

    if (frame.isACK)
    {
        int ack = mod(frame.sequenceNumber, SEQ_MOD);

        if (protocol == ReliableProtocol::GBN)
        {
            if (!seqInUnacked(ack, base, nextSeq, SEQ_MOD))
            {
                cout << "[" << id << "] ignoring stale/invalid cumulative ACK SEQ="
                     << ack << endl;
                return;
            }
            base = mod(ack + 1, SEQ_MOD);
            cout << "[" << id << "] GBN: cumulative ACK through SEQ=" << ack
                 << ", new base=" << base << endl;
            return;
        }

        sr_acked[ack] = true;
        while (sr_acked[base])
        {
            sr_acked[base] = false;
            base = mod(base + 1, SEQ_MOD);
        }
        cout << "[" << id << "] SR: selective ACK for SEQ=" << ack
             << ", new base=" << base << endl;
        return;
    }

    if (!crc16_verify(frame.payload, frame.crc))
    {
        cout << "[ERROR] CRC mismatch — frame discarded\n";
        return;
    }

    int seq = mod(frame.sequenceNumber, SEQ_MOD);

    if (protocol == ReliableProtocol::GBN)
    {
        cout << "[" << id << "] GBN received frame SEQ=" << seq << " from "
             << sender->getId() << endl;

        if (seq != expectedSeq)
        {
            cout << "[" << id << "] GBN: out-of-order (expected " << expectedSeq
                 << "), discarding\n";
            if (expectedSeq != 0)
            {
                int dup = mod(expectedSeq - 1, SEQ_MOD);
                Frame dupAck(macAddress, frame.sourceMAC, "", dup, true);
                cout << "[" << id << "] re-sending duplicate cumulative ACK SEQ="
                     << dup << endl;
                AckBuffer::buffer.push(dupAck);
            }
            return;
        }

        cout << "[SUCCESS] Frame received correctly\n";
        cout << "Payload: " << frame.payload << endl;

        // if upper layer data present, show application handling
        if (frame.payload.find("HTTP") != string::npos)
            appHTTP_Server(frame.payload);
        if (frame.payload.find("DNS") != string::npos)
            appDNS_Reply("host", getIP());

        Frame ack(macAddress, frame.sourceMAC, "", seq, true);
        cout << "[" << id << "] GBN: ACK (cumulative through SEQ=" << seq << ")\n";
        AckBuffer::buffer.push(ack);
        expectedSeq = mod(expectedSeq + 1, SEQ_MOD);
        return;
    }

    cout << "[" << id << "] SR received frame SEQ=" << seq << " from "
         << sender->getId() << endl;

    if (!inRecvWindow(seq, rcv_base, windowSize, SEQ_MOD))
    {
        cout << "[" << id << "] SR: SEQ outside receive window, discarding\n";
        return;
    }

    if (sr_recvFilled[seq])
    {
        Frame ack(macAddress, frame.sourceMAC, "", seq, true);
        cout << "[" << id << "] SR: duplicate frame, re-ACK SEQ=" << seq << endl;
        AckBuffer::buffer.push(ack);
        return;
    }

    sr_recvPayload[seq] = frame.payload;
    sr_recvFilled[seq] = true;

    Frame ack(macAddress, frame.sourceMAC, "", seq, true);
    cout << "[" << id << "] SR: selective ACK SEQ=" << seq << endl;
    AckBuffer::buffer.push(ack);

    while (sr_recvFilled[rcv_base])
    {
        cout << "[SUCCESS] SR delivering in-order SEQ=" << rcv_base << "\n";
        cout << "Payload: " << sr_recvPayload[rcv_base] << endl;
        sr_recvFilled[rcv_base] = false;
        sr_recvPayload[rcv_base].clear();
        rcv_base = mod(rcv_base + 1, SEQ_MOD);
    }
}
