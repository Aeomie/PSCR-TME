
#include "Pool.h"
namespace pr
{
    Pool::Pool(int qsize, pr::Barrier *barrier) : queue(qsize), barrier(barrier) {};
    void Pool::start(int nbthread)
    {
        for (int i = 0; i < nbthread; ++i)
        {
            threads.emplace_back();
        }
    }
    void Pool::submit(Job *job)
    {
        queue.push(job);
    }
    void Pool::stop()
    {
        queue.setBlocking(false);
        for (auto &t : threads)
        {
            t.join();
        }
    };
    ;

    void Pool::poolWorker(Queue<Job> &queue)
    {
        while (true)
        {
            Job *j = queue.pop();
            if (j == nullptr)
            {
                return;
            }
            j->run();
            delete j;
        }
    }
    Pool::~Pool()
    {
        queue.~Queue();
    }
}