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
#include <time.h>
#include <atomic>

using namespace std;

#define BUFFERSIZE 256

class BufferNReadProc
{
    sem_t semEmpty, semFull, semMutex, semCount;
    char buffer[BUFFERSIZE]{};
    const int N;
    std::atomic<time_t> last_activity;

public:
    explicit BufferNReadProc(int n) : N(n)
    {
        sem_init(&semEmpty, 1, 1); // Start with buffer empty
        sem_init(&semFull, 1, 0);  // No data initially
        sem_init(&semMutex, 1, 1); // Mutex for buffer
        sem_init(&semCount, 1, N); // Allow `N` reads
        last_activity = time(nullptr);  
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
        update_activity();

        sem_post(&semMutex); // Unlock buffer
        sem_post(&semFull);  // Notify data is available
    }

    void read(char *msg)
    {
        sem_wait(&semCount); // Wait until reads are allowed
        sem_wait(&semFull);  // Wait for available data
        sem_wait(&semMutex); // Lock buffer access

        memcpy(msg, buffer, BUFFERSIZE); // Read message
        update_activity();

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

    static BufferNReadProc *createBuffer(const char *name, const int &n)
    {
        int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666);
        if (fd < 0)
        {
            return NULL;
        }
        ftruncate(fd, sizeof(BufferNReadProc));

        void *shm = mmap(0, sizeof(BufferNReadProc), PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        BufferNReadProc *buffer = new (shm) BufferNReadProc(n); // creates it on shm

        closeBuffer(buffer); // to make sure that it doesnt stay open
        return buffer;
    }

    static void deleteBuffer(const char *name)
    {
        shm_unlink(name); // accesses it by name and deletes it
    }

    // Close the stack by unmapping the memory
    static void closeBuffer(BufferNReadProc *buffer)
    {
        // 'stack' is a pointer to the memory region
        munmap(buffer, sizeof(BufferNReadProc)); // Unmap the stack memory
    }

    static BufferNReadProc *openBuffer(const char *name)
    {
        int fd = shm_open(name, O_RDWR, 0666);
        void *shm = mmap(0, sizeof(BufferNReadProc), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        // Renvoie un pointeur vers le Stack<T> situé à l'adresse mappée.
        return static_cast<BufferNReadProc *>(shm);
    }
    void update_activity() {
        last_activity = time(nullptr);
    }

    time_t get_last_activity() const {
        return last_activity;
    }
};
