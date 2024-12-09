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

using namespace std;

#define BUFFERSIZE 256

class BufferNReadProc
{
    sem_t semEmpty, semFull, semMutex, semCount;
    char buffer[BUFFERSIZE]{};
    const int N;

public:
    explicit BufferNReadProc(int n) : N(n)
    {
        sem_init(&semEmpty, 1, 1); // Start with buffer empty
        sem_init(&semFull, 1, 0);  // No data initially
        sem_init(&semMutex, 1, 1); // Mutex for buffer
        sem_init(&semCount, 1, N); // Allow `N` reads
    }

    ~BufferNReadProc()
    {
        sem_destroy(&semEmpty);
        sem_destroy(&semFull);
        sem_destroy(&semMutex);
        sem_destroy(&semCount);
    }

    void write(const char *msg)
    {
        sem_wait(&semEmpty); // Wait for buffer space
        sem_wait(&semMutex); // Lock buffer access

        memcpy(buffer, msg, BUFFERSIZE); // Write message

        sem_post(&semMutex); // Unlock buffer
        sem_post(&semFull);  // Notify data is available
    }

    void read(char *msg)
    {
        sem_wait(&semCount); // Wait until reads are allowed
        sem_wait(&semFull);  // Wait for available data
        sem_wait(&semMutex); // Lock buffer access

        memcpy(msg, buffer, BUFFERSIZE); // Read message

        sem_post(&semMutex); // Unlock buffer
        sem_post(&semEmpty); // Signal buffer space available
    }

    static void producer(BufferNReadProc *buffer, const char *msg)
    {
        buffer->write(msg);
        cout << "Producer wrote :  " << msg << endl;
    }

    // Consumer function
    static void consumer(BufferNReadProc *buffer)
    {
        char msg[BUFFERSIZE];
        buffer->read(msg);
        cout << "Consumer read: " << msg << endl;
    }
};
