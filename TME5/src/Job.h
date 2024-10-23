#pragma once
#include <iostream>
#include <thread>
#include "Barrier.h"
namespace pr
{

	class Job
	{
	public:
		virtual void run() = 0;
		virtual ~Job() {};
	};

	// Job concret : exemple

	/**
	class SleepJob : public Job {
		int calcul (int v) {
			std::cout << "Computing for arg =" << v << std::endl;
			// traiter un gros calcul
			this_thread::sleep_for(1s);
			int ret = v % 255;
			std::cout << "Obtained for arg =" << arg <<  " result " << ret << std::endl;
			return ret;
		}
		int arg;
		int * ret;
	public :
		SleepJob(int arg, int * ret) : arg(arg), ret(ret) {}
		void run () {
			* ret = calcul(arg);
		}
		~SleepJob(){}
	};
	**/

	class DrawJobBarrier : public Job
	{
		int calcul(int v)
		{
			std::cout << "Computing for arg =" << v << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			int ret = v % 255;
			std::cout << "Obtained for arg =" << arg << " result " << ret << std::endl;
			return ret;
		}
		int arg;
		int *ret;
		pr::Barrier *barrier;

	public:
		DrawJobBarrier(pr::Barrier *barrier, int arg, int *ret) : arg(arg), ret(ret), barrier(barrier) {}
		void run()
		{
			*ret = calcul(arg);
			barrier->done();
		}
		~DrawJobBarrier() {}
	};
}
