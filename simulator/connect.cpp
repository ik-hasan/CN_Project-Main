#include "connect.h"
#include "../devices/Device.h"

void linkDevices(Device *a, Device *b)
{
    a->connect(b);
    b->connect(a);
}
