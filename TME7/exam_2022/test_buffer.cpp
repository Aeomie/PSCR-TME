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

// Producer function
void producer(BufferNReadProc *buffer)
{
    const char *messages[] = {"Message 1", "Message 2", "Message 3"};
    for (auto &msg : messages)
    {
        buffer->write(msg);
        cout << "Producer wrote :  " << msg << endl;
        sleep(1); // Simulate work
    }
}

// Consumer function
void consumer(BufferNReadProc *buffer, int id)
{
    for (int i = 0; i < 3; ++i)
    { // Adjust as needed
        char msg[BUFFERSIZE];
        buffer->read(msg);
        cout << "Consumer " << id << " read: " << msg << endl;
        sleep(2); // Simulate processing time
    }
}
std::vector<pid_t> tokill;

void killem(int)
{
    for (auto p : tokill)
    {
        kill(p, SIGINT);
    }
}

int main(int argc, char *argv[])
{

    shm_unlink("/buffer");       // incase it crashed before
    const int N = atoi(argv[1]); // Number of reads per message
    cout << " 0" << argv[0] << endl;
    cout << " 0" << argv[1] << endl;
    size_t shmsize = sizeof(BufferNReadProc);
    cout << "Allocating shared memory segment of size " << shmsize << endl;

    int fd;
    void *addr;

    // Create and map shared memory
    fd = shm_open("/buffer", O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd < 0)
    {
        perror("shm_open");
        return 1;
    }
    if (ftruncate(fd, shmsize) != 0)
    {
        perror("ftruncate");
        return 1;
    }
    addr = mmap(nullptr, shmsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    // In-place new to initialize shared buffer
    auto *buffer = new (addr) BufferNReadProc(N);

    // Fork producer process
    pid_t pp = fork();
    if (pp == 0)
    {
        producer(buffer);
        return 0;
    }
    tokill.push_back(pp);

    // Fork multiple consumer processes
    for (int i = 0; i < N; ++i)
    {
        pid_t pc = fork();
        if (pc == 0)
        {
            consumer(buffer, i + 1);
            return 0;
        }
        else
        {
            tokill.push_back(pc);
        }
    }

    // Set up signal handler for cleanup
    signal(SIGINT, killem);

    wait(nullptr); // Wait for producer
    wait(nullptr); // Wait for consumer

    // Cleanup shared buffer
    buffer->~BufferNReadProc();
    if (munmap(addr, shmsize) != 0)
    {
        perror("munmap");
    }
    if (shm_unlink("/buffer") != 0)
    {
        perror("shm_unlink");
    }

    return 0;
}