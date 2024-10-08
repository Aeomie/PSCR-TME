#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include "hashmap.h"
#include "TME3.h"
#include "Personne.h"
#include <unordered_map>
using namespace std;
using namespace std::chrono;

int main()
{
#pragma region TME2

	ifstream input = ifstream("./WarAndPeace.txt");
	auto start = steady_clock::now();

	cout << "Parsing War and Peace" << endl;

	// Partie 2

	unordered_map<string, int> map_words(10);

	// QS8
	unordered_map<int, forward_list<string>> frequence_map(100);

	// QS9
	vector<Personne> person_vec;
	unordered_map<int, forward_list<Personne>> personne_frequency;
	size_t nombre_lu = 0;

	// Question 3
	vector<pair<string, int>> pair_vector;
	// QS7
	std::vector<pair<string, int>> hashmap_vector;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re(R"([^a-zA-Z])");
	while (input >> word)
	{

		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace(word, re, "");

		// passe en lowercase
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		// word est maintenant "tout propre"

		bool found_in_pair = false;
		for (auto &pair : pair_vector)
		{
			if (pair.first == word)
			{
				pair.second++;
				found_in_pair = true;
				break;
			}
		}
		if (!found_in_pair)
		{
			pair_vector.push_back(make_pair(word, 1));
		}
		// QS7
		auto result = map_words.insert({word, 1});
		if (result.second)
		{
			map_words[word] = 1;
		}
		else
		{
			map_words[word]++;
		}
		person_vec.push_back(Personne(word, word, nombre_lu / 10000, 'M'));
		// QS8

		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": " << word << endl;
		nombre_lu++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
	cout << "Parsing took "
		 << duration_cast<milliseconds>(end - start).count()
		 << "ms.\n";

	cout << "Found a total of " << nombre_lu << " words." << endl;

	cout << "hash map result 'war' : " << map_words["war"] << endl;
	cout << "hash map result 'peace' : " << map_words["peace"] << endl;

#pragma endregion
#pragma region TME3

	cout << "---------------- TME3 PART2 -------------------" << endl;
	cout << "pair vector size : " << pair_vector.size() << endl;
	if (map_words.size() != 0)
		cout << "unordered map size : " << map_words.size() << endl; // test using vector

	// qs 8
	for (auto &pair : pair_vector)
	{
		string word = pair.first;
		int frequency = pair.second;

		frequence_map[frequency].push_front(word);
	}

	if (frequence_map.size() > 0)
	{
		cout << "frequency map size : " << frequence_map.size() << endl;
		for (const auto &entry : frequence_map)
		{
			int frequency = entry.first;
			const forward_list<string> &words = entry.second; // List of words
			// Iterate through the forward_list
			for (const auto &word : words)
			{
				if (word == "war" || word == "toto" || word == "peace")
				{
					cout << "frequency of : " << word << " is : " << frequency << endl;
				}
			}
		}
	}

	// qs9
	// What we have to do is , in the place of frequency in question 8 , we should exchange it with the filter that we want to use so for this case age
	for (auto person : person_vec)
	{
		personne_frequency[person.age].push_front(person);
	}
	if (personne_frequency.size() > 0)
	{

		for (const auto &entry : personne_frequency)
		{
			int age = entry.first;
			const forward_list<Personne> &persons = entry.second;
			for (const auto &person : persons)
			{
				if (age == 55)
				{
					cout << " Person name : " << person.nom << " and prenom : " << person.prenom << " + age : " << age << endl;
				}
			}
		};
	}
#pragma endregion
	return 0;
}
