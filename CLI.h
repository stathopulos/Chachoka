#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <limits>
#include "Game.h"

// Exception Classes
class InvalidInputException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Invalid input detected.";
	}
};
class InsufficientPlayersException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "At least two player names are required.";
	}
};

class CLI {
private:
	// Array of all 9 melds
	std::string meld[9] = {
		"1 Set",
		"1 Run",
		"2 Sets",
		"1 Run & 1 Set",
		"2 Runs",
		"3 Sets",
		"1 Run & 2 Sets",
		"2 Runs & 1 Set",
		"3 Runs"
	};
	bool savedGameExists() const;

public:
	CLI();
	
	// Utility functions
	void clear();
	void clearInputBuffer();
	GameParams parseGameFile(const std::string& filename);

	// Prompting user
	std::vector<std::string> promptForPlayers();
	ScoringObj promptScores(PlayerList&);
	void endOfRound(Game&);
	char loadOrCreate();
	
	// Display
	void displayGame(Game&);
	void printScores(PlayerList&);
	void displayFinalScore(Game&);
};

#endif