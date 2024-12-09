#pragma once

// size_t,memset
#include <cstring>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h> // for close & truncate
namespace pr
{

#define STACKSIZE 100

	template <typename T>
	class Stack
	{
		T tab[STACKSIZE];
		size_t sz;
		sem_t sem_Mut;
		sem_t sem_Empty;
		sem_t sem_Full;

	public:
		Stack() : sz(0)
		{
			sem_init(&sem_Mut, 1, 1);
			sem_init(&sem_Empty, 1, STACKSIZE); // Initial empty slots: STACKSIZE
			sem_init(&sem_Full, 1, 0);			// Initial full slots: 0
			memset(tab, 0, sizeof(tab));
		}

		~Stack()
		{
			sem_destroy(&sem_Mut);
			sem_destroy(&sem_Empty);
			sem_destroy(&sem_Full);
		}
		void push(T elt)
		{
			sem_wait(&sem_Empty); // Wait for an empty slot
			sem_wait(&sem_Mut);	  // Lock for mutual exclusion

			// Push the element onto the stack
			tab[sz++] = elt;

			sem_post(&sem_Mut);	 // Unlock mutual exclusion
			sem_post(&sem_Full); // Signal that there is a full slot
		}
		T pop()
		{
			sem_wait(&sem_Full); // Wait for a full slot
			sem_wait(&sem_Mut);	 // Lock for mutual exclusion

			// Pop the element from the stack
			T toret = tab[--sz];

			sem_post(&sem_Mut);	  // Unlock mutual exclusion
			sem_post(&sem_Empty); // Signal that there is an empty slot

			return toret;
		}

		static Stack<T> *createStack(const char *name)
		{
			int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666);
			if (fd < 0)
			{
				return NULL;
			}
			ftruncate(fd, sizeof(Stack<T>));

			void *shm = mmap(0, sizeof(Stack<T>), PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

			Stack<T> *stack = new (shm) Stack(); // creates it on shm

			closeStack(stack); // to make sure that it doesnt stay open
			return stack;
		}

		static void deleteStack(const char *name)
		{
			shm_unlink(name); // accesses it by name and deletes it
		}
		// Close the stack by unmapping the memory
		static void closeStack(Stack<T> *stack)
		{
			// 'stack' is a pointer to the memory region
			munmap(stack, sizeof(Stack<T>)); // Unmap the stack memory
		}

		Stack<T> *openStack(const char *name)
		{
			int fd = shm_open(name, O_RDWR, 0666);
			void *shm = mmap(0, sizeof(Stack<T>), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			close(fd);
			// Renvoie un pointeur vers le Stack<T> situé à l'adresse mappée.
			return static_cast<Stack<T> *>(shm);
		}
	};

}
