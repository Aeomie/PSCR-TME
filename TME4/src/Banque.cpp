#include "Banque.h"

#include <iostream>
using namespace std;

namespace pr
{

	void Banque::transfert(int nb_loop)
	{
		std::unique_lock<std::recursive_mutex> lock(this->getMutex());
		while (nb_loop > 0)
		{
			int cred_i = threadRandom(0, comptes.size() - 1);
			int deb_i = threadRandom(0, comptes.size() - 1);

			while (deb_i == cred_i)
			{
				deb_i = threadRandom(0, comptes.size() - 1);
			}
			int m = threadRandom(0, 100);
			int sleep_time = threadRandom(0, 20);

			Compte &debiteur = comptes[deb_i];
			Compte &crediteur = comptes[cred_i];

			if (deb_i < cred_i)
			{
				std::unique_lock<std::recursive_mutex> lock_d(debiteur.getMutex());
				std::unique_lock<std::recursive_mutex> lock_c(crediteur.getMutex());
			}
			else
			{
				std::unique_lock<std::recursive_mutex> lock_c(crediteur.getMutex());
				std::unique_lock<std::recursive_mutex> lock_d(debiteur.getMutex());
			}

			// Process the transfer
			if (debiteur.debiter(m))
			{
				crediteur.crediter(m);
			}
			nb_loop--;
		}
	}

	size_t Banque::size() const
	{
		return comptes.size();
	}
	bool Banque::comptabiliser(int attendu) const
	{
		int bilan = 0;
		int id = 0;
		std::unique_lock<std::recursive_mutex> lock(mutex);
		for (const auto &compte : comptes)
		{
			if (compte.getSolde() < 0)
			{
				cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
			}
			bilan += compte.getSolde();
			id++;
		}
		if (bilan != attendu)
		{
			cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
		}
		return bilan == attendu;
	}
	int Banque::getSolde(int index)
	{
		return comptes.at(index).getSolde();
	}
	int Banque::threadRandom(int min, int max)
	{
		static thread_local std::mt19937 gen;
		std::uniform_int_distribution<int> d(min, max);
		return d(gen);
	}
	std::recursive_mutex &Banque::getMutex()
	{
		return mutex;
	}
}
