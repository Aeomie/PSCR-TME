#include <iostream>
#include "Stack.h"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <semaphore.h>
#include <fcntl.h>	  // For O_* constants
#include <sys/stat.h> // For mode constants

using namespace std;
using namespace pr;

void producteur(Stack<char> *stack)
{
	char c;
	while (cin.get(c))
	{
		stack->push(c);
	}
	// Push a special end-of-data signal
	stack->push('\0');
}

void consomateur(Stack<char> *stack)
{
	while (true)
	{
		char c = stack->pop();
		if (c == '\0')
		{
			break; // Exit the loop on end signal
		}
		cout << c << flush;
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

int main()
{

	Stack<char> *s = Stack<char>::createStack("/stack");
	if (!s)
	{
		cerr << "Failed to create shared stack" << endl;
		return 1;
	}

	s->openStack("/stack");
	// Producer process
	pid_t pp = fork();
	if (pp == 0)
	{
		producteur(s);
		return 0;
	}

	// Consumer process
	pid_t pc = fork();
	if (pc == 0)
	{
		consomateur(s);
		return 0;
	}
	else
	{
		tokill.push_back(pc);
	}

	signal(SIGINT, killem);

	wait(nullptr); // Wait for producer
	wait(nullptr); // Wait for consumer

	// Cleanup
	s->~Stack();
	Stack<char>::closeStack(s);
	Stack<char>::deleteStack("/stack");

	return 0;
};
