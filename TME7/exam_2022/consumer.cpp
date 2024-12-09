#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <csignal>

#include "bufferNreadproc.h"
int main()
{
    size_t shmsize = sizeof(BufferNReadProc);

    // Open the shared memory
    int fd = shm_open("/mybuffer", O_RDWR, 0666);
    if (fd < 0)
    {
        perror("shm_open");
        return 1;
    }

    // Map the shared memory into the address space
    void *addr = mmap(nullptr, shmsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    // Initialize the shared memory structure
    auto *buffer = static_cast<BufferNReadProc *>(addr);

    // Consumer reads from the buffer
    buffer->consumer(buffer);

    // Cleanup: just munmap the shared memory (don't unlink here)
    buffer->~BufferNReadProc();
    if (munmap(addr, shmsize) != 0)
    {
        perror("munmap");
    }

    if (shm_unlink("/mybuffer") != 0)
    {
        perror("unlink");
    }
    return 0;
}