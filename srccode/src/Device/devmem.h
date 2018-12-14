#pragma once
/*---------------------------------------------------------------------------*/
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "Poco/NObserver.h"

namespace DeviceIO
{
/*---------------------------------------------------------------------------*/
// #include <vector>
/*---------------------------------------------------------------------------*/
class MemoryDevice
{
public:
    MemoryDevice();
    ~MemoryDevice();
    void dev_mem_write(off_t target, unsigned int data);
    unsigned int dev_mem_read(off_t target);
private:
    static Poco::Mutex mutexDevMemory;

};
} // namespace DeviceIO

