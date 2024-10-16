#pragma once

#include "Compte.h"
#include <thread>
#include <vector>
#include <random>

namespace pr
{

	class Banque
	{
		typedef std::vector<Compte> comptes_t;
		comptes_t comptes;
		mutable std::recursive_mutex mutex;

	public:
		Banque(size_t ncomptes, size_t solde) : comptes(ncomptes, Compte(solde))
		{
		}
		void transfert(int nb_loop);
		size_t size() const;
		bool comptabiliser(int attendu) const;
		int getSolde(int index);
		int threadRandom(int min, int max);
		std::recursive_mutex &getMutex();
	};

}
