#include <iostream>
#include <mutex>
#include <condition_variable>

class Barrier
{
    int max;
    int cur;
    std::mutex m;
    std::condition_variable cv;

public:
    Barrier(int max) : max(max), cur(0) {}
    // Copy constructor
    Barrier(const Barrier &other)
        : max(other.max), cur(other.cur) {};
    void done()
    {
        std::unique_lock<std::mutex> lock(m);
        cur++;
        if (cur >= max)
        {
            cv.notify_all();
        }
    }

    void waitFor()
    {
        std::unique_lock<std::mutex> lock(m);
        while (cur < max)
        {
            cv.wait(lock);
        }
    }
};
