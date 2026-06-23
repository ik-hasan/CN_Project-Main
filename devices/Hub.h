// #ifndef HUB_H
// #define HUB_H

// #include "Device.h"

// class Hub : public Device {
// public:
//     Hub(string id);

//     // hub receives and broadcasts
//     void receive(const string& data, Device* sender) override;
// };

// #endif




//Commented above code in step 4
#ifndef HUB_H
#define HUB_H

#include "Device.h"

class Hub : public Device {

public:
    Hub(string id);
    // UPDATED to use Frame
    void receive(Frame frame, Device* sender) override;

};

#endif