#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring> // pour memcpy

#include <iostream>
#include <vector>
#include <thread>
#include <cstring>

// BufferNRead class definition remains unchanged
class BufferNRead
{
    std::mutex mtx;
    std::condition_variable canWrite, canRead;
    char buffer[256]{};
    int readCount = 0;
    const int N;
    bool hasMessage = false;

public:
    BufferNRead(int n) : N(n) {}

    void write(const char *msg)
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (hasMessage)
        {
            canWrite.wait(lock);
        }
        std::memcpy(buffer, msg, 256);
        readCount = 0;
        hasMessage = true;
        canRead.notify_all();
    }

    void read(char *msg)
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (!hasMessage || readCount >= N)
        {
            canRead.wait(lock);
        }
        std::memcpy(msg, buffer, 256);
        readCount++;
        if (readCount == N)
        {
            hasMessage = false;
            canWrite.notify_one();
        }
    }
};

// Example of dynamic thread usage
int main()
{
    const int N = 3; // Number of reads per message
    BufferNRead buffer(N);
    const int numConsumers = 3; // Number of consumers
    const char *messages[] = {"Message 1", "Message 2", "Message 3"};

    // Create a vector to hold threads
    std::vector<std::thread> threads;

    // Producer thread
    std::cout << "hey" << std::endl;
    for (int i = 0; i < 3; i++)
    { // messages size
        threads.emplace_back(&BufferNRead::write, &buffer, messages[i]);
    }

    // Consumer threads
    for (int i = 0; i < numConsumers; ++i)
    {
        threads.emplace_back([&buffer, i]()
                             {
            char msg[256];
            for (int j = 0; j < 3; ++j) {
                buffer.read(msg);
                std::cout << "Consumer " << i + 1 << " read: " << msg << std::endl;
            } });
    }

    // Join all threads
    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
