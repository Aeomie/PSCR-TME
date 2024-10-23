#pragma once

#include <barrier>
#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <mutex>
namespace pr
{
    class Barrier
    {
        int max;
        int cur;
        std::mutex m;
        std::condition_variable cond;

        Barrier(int max) : max(max), cur(0) {}

    public:
        void done()
        {
            std::unique_lock<std::mutex> l(m);
            cur++;
            if (cur >= max)
            {
                cond.notify_all();
            }
        }

        void waitFor()
        {
            std::unique_lock<std::mutex> l(m);
            while (cur != max)
            {
                cond.wait(l);
            }
        }
    };

}
