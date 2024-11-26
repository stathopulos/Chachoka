#include "Game.h"

// Default constructor with placeholders
Game::Game() : Players(std::vector<std::string> {"Player 1", "Player 2"}), round(1) {}
// Constructor for new game from player names
Game::Game(const std::vector<std::string>& names) : Players(names), round(1) {}
// Constructor for creating game object from a save file
Game::Game(const GameParams& params) : round(params.roundNum), Players(params.names) {
    Players.setCutter(params.cutterName);
    Players.setDealer(params.dealerName);
    Players.setHead(params.headName);

    // Set initial scores for players
    Player* currentPlayer = Players.getHead();
    for (size_t i = 0; i < params.scores.size(); ++i) {
        if (currentPlayer == nullptr) {
            std::cerr << "Error: Not enough players in the list to set scores." << std::endl;
            break;
        }

        currentPlayer->setScore(params.scores[i]);

        // Move to the next player in the list
        currentPlayer = currentPlayer->getNext();
    }
}

Game::~Game() {
    Player* current = Players.getHead();
    while (current != nullptr) {
        Player* temp = current;
        current = current->getNext();
        delete temp;
    }
}

// Getters
int Game::getRound() const {
    return round;
}
PlayerList& Game::getPlayerList() {
    return Players;
}

// Modifiers
void Game::nextRound() {
    Players.nextDealer();
    Players.nextCutter();
    round++;
}
void Game::scoreRound(ScoringObj scoring) {
    if (scoring.winner == nullptr) {
        std::cerr << "Error: Invalid starting player." << std::endl;
        return;
    }

    // Subtract 10 from cutter's score if the cut was successful
    if (scoring.isCutSuccessful) {
        Players.getCutter()->subtractPoints(10);
    }

    // Subtract 10 from the winning player's score
    scoring.winner->subtractPoints(10);

    // Initialize a variable to keep track of the current player
    Player* currentPlayer = scoring.winner->getNext();

    for (size_t i = 0; i < scoring.scores.size(); ++i) {
        if (currentPlayer == nullptr) {
            std::cerr << "Error: Not enough players in the list to distribute scores." << std::endl;
            break;
        }

        // Add the score from the vector to the current player's score
        currentPlayer->addPoints(scoring.scores[i]);

        // Move to the next player in the list
        currentPlayer = currentPlayer->getNext();
    }
}

// Save game
void Game::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (file.is_open()) {
        // Write round count
        file << "Round: " << round << "\n---\n";

        // Write cutter, dealer, and head of PlayerList
        file << "Cutter: " << Players.getCutter()->getName() << "\n";
        file << "Dealer: " << Players.getDealer()->getName() << "\n";
        file << "Head: " << Players.getHead()->getName() << "\n---\n";

        // Write players and their scores
        Player* initialHead = Players.getHead();
        Player* currentPlayer = initialHead;

        do {
            file << currentPlayer->getName() << ": " << currentPlayer->getScore() << "\n";
            currentPlayer = currentPlayer->getNext();
        } while (currentPlayer != initialHead);

        file.close();
        std::cout << "Game data has been saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}
