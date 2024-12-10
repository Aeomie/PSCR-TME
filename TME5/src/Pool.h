#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
using namespace std;
namespace pr
{
	class Pool
	{
		Queue<Job> queue;
		vector<thread> threads;

	public:
		Pool(int qsize) : queue(qsize) {};
		void start(int nbthread)
		{
			for (int i = 0; i < nbthread; i++)
			{
				threads.emplace_back(&poolWorker, &queue);
			}
		};
		void submit(Job *job)
		{
			queue.push(job);
		};
		void stop()
		{
			queue.setBlocking(false);
			for (auto &t : threads)
			{
				t.join();
			}
			threads.clear();
		};
		void poolWorker(Queue<Job> &queue)
		{
			Job *j = queue.pop();
			//
			j->run();
			delete j;
		}
		~Pool()
		{
			queue.~Queue();
		};
	};

}
