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
    const char* buff_name = "/mybuffer";

    BufferNReadProc* buffer =BufferNReadProc::openBuffer(buff_name);

    // Consumer reads from the buffer
    buffer->consumer(buffer);

    // Cleanup: just munmap the shared memory (don't unlink here)
    buffer->~BufferNReadProc();
    BufferNReadProc::closeBuffer(buffer); // closing
    return 0;
}