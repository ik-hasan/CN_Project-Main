// #ifndef DEVICE_H
// #define DEVICE_H

// #include "../network/Frame.h"


// #include <iostream>
// #include <vector>
// #include <string>

// using namespace std;

// class Device {
// protected:
//     string id;
//     vector<Device*> connections;   // stores connected devices

// public:
//     Device(string id);

//     string getId() const;

//     // connect this device to another
//     void connect(Device* device);

//     // virtual functions (will be overridden later)
//     virtual void send(const string& data);
//     virtual void receive(const string& data, Device* sender);


//     // helper to print connections (for testing)
//     void showConnections() const;

//     virtual ~Device() {}
// };

// #endif



//Commented above code in step 4
#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <vector>
#include <string>
#include "../network/Frame.h"   // NEW include

using namespace std;

class Device {
protected:
    string id;
    vector<Device*> connections;

public:
    Device(string id);

    string getId() const;

    void connect(Device* device);

    // UPDATED: now receives Frame
    virtual void receive(Frame frame, Device* sender);

    virtual ~Device() {}
};

#endif