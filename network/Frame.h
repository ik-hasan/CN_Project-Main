#ifndef FRAME_H
#define FRAME_H

#include <cstdint>
#include <string>

#include "Crc.h"

using namespace std;

class Frame
{

public:
    string sourceMAC;
    string destinationMAC;
    string payload;
    uint16_t crc;
    int sequenceNumber;
    bool isACK;

    Frame(string src, string dest, string data, int seq, bool ack = false)
    {
        sourceMAC = src;
        destinationMAC = dest;
        payload = data;
        sequenceNumber = seq;
        isACK = ack;
        crc = crc16_ccitt(payload);
    }
};

#endif
