#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include "hashmap.h"
<<<<<<< HEAD
#include "TME3.h"
=======
>>>>>>> refs/remotes/origin/master
using namespace std;
using namespace std::chrono;

int main()
{
<<<<<<< HEAD
#pragma region TME2
=======
>>>>>>> refs/remotes/origin/master

	ifstream input = ifstream("./WarAndPeace.txt");
	auto start = steady_clock::now();

	cout << "Parsing War and Peace" << endl;

	// Partie 1
	// Question 2
	vector<string> word_saver;
	int new_word_count = 0;
	// Question 3
	vector<pair<string, int>> pair_vector;

	// Partie 1.3
	HashMap<string, int> map_words(10);
	size_t nombre_lu = 0;
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

<<<<<<< HEAD
=======
		/*
>>>>>>> refs/remotes/origin/master
		// PARTIE 1
		// QS 2 Too SLOW
		// check if there is a dup
		bool isNewWord = true;
<<<<<<< HEAD
		/*
=======
>>>>>>> refs/remotes/origin/master
		for (string vector_word : word_saver)
		{
			if (vector_word == word)
			{
				isNewWord = false;
				break;
			}
		}
<<<<<<< HEAD
		*/
=======
>>>>>>> refs/remotes/origin/master
		if (isNewWord)
		{
			new_word_count++;
		}
		word_saver.push_back(word);

<<<<<<< HEAD
		/*
=======
>>>>>>> refs/remotes/origin/master
		// QS 3
		bool found_in_pair = false;
		for (auto &pair : pair_vector)
		{
			if (pair.first == word)
			{
				pair.second++;
				found_in_pair = true;
			}
		}
		if (!found_in_pair)
		{
			pair_vector.push_back(make_pair(word, 1));
		}
		*/
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
		// Partie 1.3
		int *valuePtr = map_words.getValue(word);
		if (valuePtr == nullptr)
		{
			map_words.insert(word, 1);
		}
		else
		{
			map_words.insert(word, *valuePtr + 1);
		}

		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": " << word << endl;
		nombre_lu++;
<<<<<<< HEAD
		if (nombre_lu == 100000)
		{
			break;
		}
=======
>>>>>>> refs/remotes/origin/master
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
	cout << "Parsing took "
		 << duration_cast<milliseconds>(end - start).count()
		 << "ms.\n";

	cout << "Found a total of " << nombre_lu << " words." << endl;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	/*
	// PARIIE 1
	//  QS2
	cout << "Vector size : " << word_saver.size() << endl;
	cout << "Nb new words : " << new_word_count << endl;
	cout << "Nombre d'occurence du mot war : ";

<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	// QS3
	for (auto &pair : pair_vector)
	{
		if (pair.first == "war" || pair.first == "peace" || pair.first == "toto")
		{
			cout << "Nombre d'occurence du mot " + pair.first + "est : " + to_string(pair.second) << endl;
		}
	}
	*/

	// Partie 1.3

	// QS6
	cout << "---------------- QUESTION 6 -------------------" << endl;
	cout << "hash map result 'war' : " << *map_words.getValue("war") << endl;
	cout << "hash map result 'peace' : " << *map_words.getValue("peace") << endl;
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	// QS7 switching it to vector
	for (const auto &bucket : map_words.buckets)
	{
		for (const auto &entry : bucket)
		{
			hashmap_vector.emplace_back(entry.key, entry.value);
		}
	}

<<<<<<< HEAD
	/*
	// QS8
	std::sort(hashmap_vector.begin(), hashmap_vector.end(), [](const pair<string, int> &a, const pair<string, int> &b)
			  { return a.second > b.second; });
=======
	// QS8
	std::sort(hashmap_vector.begin(), hashmap_vector.end(), [](const pair<string, int> &a, const pair<string, int> &b)
			  { return a.second > b.second; });

>>>>>>> refs/remotes/origin/master
	// to print only top 20
	cout << "---------------- QUESTION 8 -------------------" << endl;
	int i = 10;
	for (const auto &pair : hashmap_vector)
	{
		if (i > 0)
		{
			cout << "Name: " << pair.first << " Occurrences: " << pair.second << endl;
			i--;
		}
	}
<<<<<<< HEAD
	*/
#pragma endregion
#pragma region TME3

	cout << "---------------- TME3 PART1 -------------------" << endl;
	if (word_saver.size() != 0)
		cout << "Count using VECTOR generic size_t function in tme3 : " << tme3::count(word_saver.begin(), word_saver.end()) << endl; // test using vector
	cout << "Count using VECTOR generic size_t function in tme3 :  Value searched = " << " war count = " << tme3::count_if_equal(word_saver.begin(), word_saver.end(), string("war")) << endl;

	if (map_words.size() != 0)
		cout << "Count using  HASHMAP generic size_t function in tme3 : " << tme3::count(map_words.begin(), map_words.end()) << endl; // test using vector

	hashmap_vector.clear();
	for (const auto &bucket : map_words.buckets)
	{
		for (const auto &entry : bucket)
		{
			hashmap_vector.emplace_back(entry.key, entry.value);
		}
	}
	std::sort(hashmap_vector.begin(), hashmap_vector.end(), [](const pair<string, int> &a, const pair<string, int> &b)
			  { return a.second > b.second; });
	// to print only top 20
	int i = 10;
	for (const auto &pair : hashmap_vector)
	{
		if (i > 0)
		{
			cout << "Name: " << pair.first << " Occurrences: " << pair.second << endl;
			i--;
		}
	}
#pragma endregion
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}
