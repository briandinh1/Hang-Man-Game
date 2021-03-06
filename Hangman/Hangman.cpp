#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <random>


// return index of a random character based on its weighted frequency
int getRandom(const std::vector<int>& frequency)
{
	std::discrete_distribution<int> distribution{ frequency.begin(), frequency.end() };
	std::random_device rd;
	std::mt19937 engine(rd());
	return distribution(engine);
}

std::string createWord(const std::vector<int>& frequency, int wordLength)
{
	std::string word = "";
	for (int i = 0; i < wordLength; i++)
		word += 'A' + getRandom(frequency);
	return word;
}

void draw(const std::string& word, const std::vector<bool>& matched, bool endGame = false)
{
	for (int i = 0; i < matched.size(); i++)
		std::cout << (matched[i] || endGame ? word[i] : '_') << ' ';
	std::cout << '\t';
}

bool guessLetter(char c, const std::string& word, std::vector<bool>& matched)
{
	bool match = false;
	for (int i = 0; i < word.size(); i++)
		if (toupper(c) == word[i] && !matched[i])
			match = matched[i] = true;
	return match;
}

int checkWordLength(std::string wordLength)
{
	int firstDigit = 0;
	int secondDigit = 0;

	// if first character is not a number, or 0, return default length
	if (firstDigit == '0' || isalpha(wordLength[0])) return 10;

	firstDigit = wordLength[0] - '0';
	if (wordLength.size() > 1)
	{
		// if second character is not a number, but first is, return first digit
		if (isalpha(wordLength[1])) return firstDigit;

		// if both digits are numbers, but the first makes a number > 20, return 20
		secondDigit = wordLength[1] - '0';
		if (firstDigit > 2 && secondDigit > 0) return 20;
	}
	
	// both numbers combined make a number < 20
	return firstDigit * 10 + secondDigit;
}

bool wonGame(const std::vector<bool>& matched)
{
	for (auto c : matched)
		if (!c) return false;
	return true;
}

void playGame(const std::vector<int>& frequency)
{
	std::string wordLength;
	std::cout << "Choose word length (default 10, limit 20):\t";
	std::cin >> wordLength;
	int length = checkWordLength(wordLength);
	int triesLeft = length;

	std::string word = createWord(frequency, length);
	std::vector<bool> matched(length, false);

	while (triesLeft > 0)
	{
		std::cout << "Choose a letter: " << std::endl;
		draw(word, matched);
		std::string guess; // string to prevent user from inputing a ton of characters into a char
		std::cin >> guess;
		std::cout << std::endl;
		if (guessLetter(guess[0], word, matched)) std::cout << "Letter Matched!";
		else std::cout << "Wrong..." << std::endl << --triesLeft << " tries left";

		std::cout << std::endl << std::endl;
	}

	if (wonGame(matched))
	{
		std::cout << "You Won! " << std::endl;
		draw(word, matched);
	}
	else
	{
		std::cout << "You lost. The word was: " << std::endl;
		draw(word, matched, true);
	}
}

int main()
{
	// frequency of character occurences based on Declaration of Independence
	std::vector<int> frequency = { 483, 95, 187, 254, 868, 183, 131, 353, 455, 17, 
		14, 230, 146, 490, 520, 138, 6, 427, 481, 649, 213, 74, 97, 9, 82, 4 };

	std::cout << "Let's play Hang Man" << std::endl << std::endl;
	while (1)
	{
		playGame(frequency);
		std::string restart;
		std::cout << std::endl << std::endl << "Do you want to play again? (Y / N)\t" << std::endl;
		std::cin >> restart;
		if (tolower(restart[0]) != 'y') break; // any character other than Y will terminate
												// doesnt necessarily have to be N
	}

    return 0;
}

