#ifndef BRIDGE_H
#define BRIDGE_H

#include "Device.h"
#include <unordered_map>

// learning bridge (Layer 2) - learns MAC and forwards
class Bridge : public Device
{
private:
    unordered_map<string, Device *> macTable;

public:
    Bridge(string id);
    void receive(Frame frame, Device *sender) override;
    void printMACTable();
};

#endif
