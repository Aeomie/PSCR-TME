#pragma once

#include <barrier>
#include "Queue.h"
#include "Job.h"
#include "Barrier.h"
#include <vector>
#include <thread>

namespace pr
{
	class Pool
	{
		Queue<Job> queue;
		std::vector<std::thread> threads;
		pr::Barrier *barrier;

	public:
		Pool(int qsize, Barrier *barrier);
		void start(int nbthread);
		void submit(Job *job);
		void stop();
		~Pool();
		void poolWorker(Queue<Job> &queue);
	};

}
