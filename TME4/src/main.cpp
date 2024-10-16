#include "Banque.h"

#include <iostream>
#include <thread>
using namespace std;

const int NB_THREAD = 10;
int main()
{
	vector<thread> threads;
	// TODO : creer des threads qui font ce qui est demandé
	pr::Banque banque = pr::Banque(25, 5000);

	for (int i = 0; i < NB_THREAD; i++)
	{
		cout << "adding to thread" << endl;
		threads.emplace_back(&pr::Banque::transfert, &banque, 5000);
		threads.emplace_back(&pr::Banque::comptabiliser, &banque, 25 * 5000);
	}
	for (auto &t : threads)
	{
		cout << "joining threads" << endl;
		t.join();
	}
	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
