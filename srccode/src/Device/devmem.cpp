#include "devmem.h"

using Poco::NObserver;
using Poco::ScopedLock;
using Poco::Mutex;

#define MAX_MSG_SIZE    1024
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
// #include <math.h>

namespace DeviceIO
{

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

Mutex MemoryDevice::mutexDevMemory;

MemoryDevice::MemoryDevice()
{     
}

MemoryDevice::~MemoryDevice()
{
}

void MemoryDevice::dev_mem_write(off_t target, unsigned int data){
    ScopedLock<Mutex> lock(mutexDevMemory);
    int fd;
    void *map_base, *virt_addr;

    unsigned int workdata1 = 0;

    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    fflush(stdout);

    // Map one page
    map_base =  mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *)
            -1) FATAL;
    fflush(stdout);

    virt_addr = map_base + (target & MAP_MASK);
//    printf("Value at address 0x%X (%p): 0x%X\n", target, virt_addr, read_result);
    fflush(stdout);

     workdata1 = (unsigned int) (data);
    *((unsigned int *) virt_addr) = workdata1;

    fflush(stdout);

    if(munmap(map_base, MAP_SIZE) == -1) FATAL;
    close(fd);
}

unsigned int MemoryDevice::dev_mem_read(off_t target){
    int fd;
    void *map_base, *virt_addr;
    unsigned int read_result = 0;
    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    fflush(stdout);

    // Map one page
    map_base =  mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *)
            -1) FATAL;
    fflush(stdout);

    virt_addr = map_base + (target & MAP_MASK);
//    printf("Value at address 0x%X (%p): 0x%X\n", target, virt_addr, read_result);
    fflush(stdout);

    read_result = *((unsigned int *) virt_addr);
        fflush(stdout);

    if(munmap(map_base, MAP_SIZE) == -1) FATAL;
    close(fd);
    return read_result;
}

}
