// #include "Device.h"

// Device::Device(string id) {
//     this->id = id;
// }

// string Device::getId() const {
//     return id;
// }

// void Device::connect(Device* device) {
//     connections.push_back(device);
// }

// void Device::send(const string& data) {
//     cout << "[" << id << "] Sending data: " << data << endl;

//     // for now just forward to all connected devices
//     for (auto device : connections) {
//         device->receive(data, this);
//     }
// }

// void Device::receive(const string& data, Device* sender) {
//     cout << "[" << id << "] Received data from [" 
//          << sender->getId() << "]: " << data << endl;
// }


// void Device::showConnections() const {
//     cout << "[" << id << "] connected to: ";
//     for (auto device : connections) {
//         cout << device->getId() << " ";
//     }
//     cout << endl;
// }





//Commented above code in step 4
#include "Device.h"

Device::Device(string id) {
    this->id = id;
}

string Device::getId() const {
    return id;
}

void Device::connect(Device* device) {
    connections.push_back(device);
}

void Device::receive(Frame frame, Device* sender) {

    cout << "[" << id << "] received frame from ["
         << sender->getId() << "]\n";

}