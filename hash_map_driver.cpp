// Caleb Catt 6/2/2019
// This program demonstrates time complexity compared amongst different types of collision handlers

#include <iostream>
#include <string>
#include "hash_map3.h"
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <time.h>

class hash_func
{
public:
	unsigned long operator() (const string& key)
	{
		unsigned long total = 0;
		const unsigned long BIG_PRIME = 4294967291;
		const int FINDS = 100000;

		for (unsigned i = 0; i < key.length(); i++)
		{
			total = total * 13 + key[i];
		}
		return total * BIG_PRIME;
	}
};

std::string underline(const std::string& s) {
	return std::string(s.length(), '-');
}

int main()
{
	typedef hash_map<string, string, hash_func> hash_map_class;
	hash_map_class words;
	hash_map_class::iterator itr;
	ifstream file("liberal.txt");
	string inputWord;

	clock_t ticks;
	ticks = clock();
	// inserting into hashmap
	while (file >> inputWord) {
		value_type<const string, string> word(inputWord, inputWord);
		pair<hash_map_class::iterator, bool> p = words.insert(word);
	}
	ticks = clock() - ticks;
	cout << "Total time (clock ticks) to read and insert " << words.size() << " items from a text file to a hash map: " << ticks << endl;

	//Setup Random Number Generator
	int randomWordIndex;
	srand(time(0));

	//Generate Array Containing words that are in the hash map
	string containedWords[500];
	string addWord;
	for (int i = 0; i <= 499; i++) 
	{
		//Get random number (to later use for index)
		randomWordIndex = rand() % ((words.size()-1) + 1);
		// Reuse our file from before
		itr = words.begin();
		// Loops through the hash map to get to random word
		int j = 0;
		while (j != randomWordIndex)
		{
			itr++;
			j++;
		}
		containedWords[i] = (*itr).first;
		file.clear(); // Clears buffer for liberal.txt
		file.seekg(0, ios::beg); // Resets to first word of liberal.txt
	}

	//Generate Array Containing words that are NOT in the hash map
	ifstream dictionaryFile("dictionary.txt");
	string uncontainedWords[500];
	inputWord = "";
	for (int i = 0; i <= 499; i++) 
	{
		dictionaryFile >> inputWord;
		itr = words.find(inputWord);
		// if word is not found, add to our array
		if (itr == words.end())
		{
			uncontainedWords[i] = inputWord;
		}
		else // If word is on list, subtract i to keep array from being null in spots
			i--;
	}
	
	// FINDS (successful)
	ticks = clock();
	for (int i = 0; i <= 199; i++)
	{
		for (int i = 0; i <= 499; i++)
			itr = words.find(containedWords[i]);
	}
	ticks = clock() - ticks;
	cout << "Total time for 100,000 random successful searches: " << ticks << endl;
	cout << "Total number of collisions over the successful searches: " << words.collisionCounter() << endl;

	// FINDS (unsuccessful)
	ticks = clock();
	for (int i = 0; i <= 199; i++)
	{
		for (int i = 0; i <= 499; i++)
			itr = words.find(uncontainedWords[i]);
	}
	ticks = clock() - ticks;
	cout << "Total time for 100,000 random unsuccessful searches: " << ticks << endl;
	cout << "Total number of collisions over the unsuccessful searches: " << words.collisionCounter() << endl;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
