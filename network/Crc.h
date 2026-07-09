#ifndef CRC_H
#define CRC_H

#include <cstdint>
#include <string>

// CRC-16-CCITT (polynomial 0x1021, init 0xFFFF) over the raw byte
// sequence of the payload string (each char is one byte).
uint16_t crc16_ccitt(const std::string &payload);

inline bool crc16_verify(const std::string &payload, uint16_t receivedCrc)
{
    return crc16_ccitt(payload) == receivedCrc;
}

#endif
