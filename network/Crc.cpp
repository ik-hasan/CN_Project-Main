#include "Crc.h"

uint16_t crc16_ccitt(const std::string &payload)
{
    uint16_t crc = 0xFFFF;
    for (unsigned char b : payload)
    {
        crc ^= static_cast<uint16_t>(b) << 8;
        for (int i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = static_cast<uint16_t>((crc << 1) ^ 0x1021);
            else
                crc <<= 1;
        }
    }
    return crc;
}
