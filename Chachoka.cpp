// Author: Koko Stathopulos
// Program: Chachoka Score Tracker
// Purpose: Allow the user to conveniently track the scores for a complete chachoka game
// Class: CSC1061
// Professor: Ralph Newby
// Date Due: 12/6/2023

#include "Game.h"
#include "CLI.h"
#include <iostream>
#include <memory>

int main() {
    CLI console = CLI(); // Initialize CLI object

    char loadOrCreate = console.loadOrCreate(); // Ask user if they want to load or create a new game

    std::unique_ptr<Game> myGame; // Use a unique_ptr to manage Game object

    if (loadOrCreate == 'L') {
        GameParams params = console.parseGameFile("myGame.txt");
        myGame = std::make_unique<Game>(params); // Load existing game

        if (params.roundNum <= 0 || params.names.empty()) {
            std::cout << "Error parsing game file or incomplete data. Creating a new game." << std::endl;
            myGame.reset(); // Reset unique_ptr to empty state to create a new game
        }
    }

    if (!myGame) { // Check if myGame is not valid
        std::vector<std::string> playerNames = console.promptForPlayers();
        myGame = std::make_unique<Game>(playerNames); // Create a new game
    }

    console.clear(); // Clear console before playing

    while (myGame->getRound() < 10) {
        console.displayGame(*myGame); // Dereference the unique_ptr to access Game object
        myGame->scoreRound(console.promptScores(myGame->getPlayerList()));
        myGame->nextRound();
        if (myGame->getRound() < 10) {
            console.endOfRound(*myGame); // Allows user to quit, save, etc
        }

        console.clear(); // Prepare for next round
    }

    console.displayFinalScore(*myGame);

    return 0;
}
