#include "List.h"
// there was no list.h include
#include <iostream>

namespace pr {

	// ******************* Chainon
	Chainon::Chainon(const std::string& data, Chainon* next) :data(data), next(next) {};

	// recursivite infinite , returning length() , fixed to returning len
	size_t Chainon::length() {
		size_t len = 1;
		if (next != nullptr) {
			len += next->length();
		}
		return len;
	}

	// no const plus the next-print was accessing after nullptr which would cause a problem
	void Chainon::print(std::ostream& os) const {
		os << data;
		if (next != nullptr) {
			os << ", ";
			next->print(os);
		}
	}

	// ******************  List

		// Constructor definition
	List::List() : tete(nullptr) {}

	// Destructor definition
	List::~List() {
		for (Chainon* c = tete; c; ) {
			Chainon* tmp = c->next;
			delete c;
			c = tmp;
		}
	}
	
	const std::string& List::operator[] (size_t index) const {
		Chainon* it = tete;
		for (size_t i = 0; i < index; i++) {
			it = it->next;
		}
		return it->data;
	}

	void List::push_back(const std::string& val) {
		if (tete == nullptr) {
			tete = new Chainon(val);
		}
		else {
			Chainon* fin = tete;
			while (fin->next) {
				fin = fin->next;
			}
			fin->next = new Chainon(val);
		}
	}

	void List::push_front(const std::string& val) {
		tete = new Chainon(val, tete);
	}

	// Forgot the List:: so the compiler wouldnt know which class to access
	bool List::empty() {
		return tete == nullptr;
	}

	size_t List::size() const {
		if (tete == nullptr) {
			return 0;
		}
		else {
			return tete->length();
		}
	}

} // namespace pr

// adding the pr:: to operator so the compiler would know which operator we're using
std::ostream& pr::operator<< (std::ostream& os, const pr::List& vec)
{
	os << "[";
	if (vec.tete != nullptr) {
		vec.tete->print(os);
	}
	os << "]";
	return os;
}

