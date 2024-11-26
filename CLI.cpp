#include "CLI.h"

CLI::CLI() {}

// Clear console, macros for windows and posix systems
void CLI::clear() {
#ifdef _WIN32
    // Windows
    system("cls");
#else
    // Assume POSIX
    system("clear");
#endif
}
// Clear input buffer to avoid errors
void CLI::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Parse the save game
GameParams CLI::parseGameFile(const std::string& filename) {
    GameParams params;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        bool roundFound = false;
        bool cutterFound = false;
        bool dealerFound = false;
        bool headFound = false;

        while (getline(file, line)) {
            if (line.find("Round: ") != std::string::npos && !roundFound) {
                std::istringstream ss(line);
                std::string roundStr;
                if (ss >> roundStr >> params.roundNum) {
                    roundFound = true;
                    continue;
                }
            }
            if (line.find("Cutter: ") != std::string::npos && !cutterFound) {
                params.cutterName = line.substr(8);
                cutterFound = true;
            }
            if (line.find("Dealer: ") != std::string::npos && !dealerFound) {
                params.dealerName = line.substr(8);
                dealerFound = true;
            }
            if (line.find("Head: ") != std::string::npos && !headFound) {
                params.headName = line.substr(6);
                headFound = true;
            }
            if (line != "---" && line.find(':') != std::string::npos) {
                size_t pos = line.find(':');
                std::string name = line.substr(0, pos);
                try {
                    int score = std::stoi(line.substr(pos + 2)); // Assuming the score follows the colon and space
                    params.names.push_back(name);
                    params.scores.push_back(score);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Error parsing score for player: " << name << ". Skipping player entry." << std::endl;
                }
            }
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    // Check if essential information is missing or invalid
    if (params.roundNum <= 0 || params.names.empty() || params.names.size() != params.scores.size() ||
        params.cutterName.empty() || params.dealerName.empty() || params.headName.empty()) {
        std::cerr << "Invalid or incomplete data in the file, no file to load." << std::endl;
        params = GameParams(); // Reset params to an empty state
    }

    return params;
}
// Check if save game exists
bool CLI::savedGameExists() const {
    std::ifstream checkFile("myGame.txt");
    return checkFile.good();
}

// Get a vector of player names by prompting for at least 2 names
std::vector<std::string> CLI::promptForPlayers() {
    std::vector<std::string> players;
    std::string playerName;

    while (true) {
        std::cout << "Enter player's names in clockwise order, starting with the "
            "first player (or leave empty to finish): ";

        std::getline(std::cin, playerName);

        if (playerName.empty()) {
            if (players.size() < 2) {
                std::cout << "Please enter at least 2 names.\n";
                continue;
            }
            else {
                break; // Exit the loop if the user leaves the input empty
            }
        }
        else if (playerName.find(':') != std::string::npos || playerName.find('\n') != std::string::npos) {
            std::cout << "Invalid input. Player name cannot contain ':' or newline characters.\n";
            continue;
        }

        players.push_back(playerName);
    }

    return players;
}
// Prompt user for all necessary information to score a round
ScoringObj CLI::promptScores(PlayerList& Players) {
    ScoringObj scoringObj;

    // Prompt for cut success
    char cutSuccessInput;
    do {
        try {
            std::cout << "Was the cut successful? (Y/N): ";
            std::cin >> cutSuccessInput;
            cutSuccessInput = std::toupper(cutSuccessInput);

            if (cutSuccessInput != 'Y' && cutSuccessInput != 'N') {
                throw InvalidInputException();
            }
        }
        catch (const InvalidInputException& e) {
            std::cout << e.what() << " Please enter Y or N.\n";
            clearInputBuffer();
        }
    } while (cutSuccessInput != 'Y' && cutSuccessInput != 'N');

    clearInputBuffer();

    scoringObj.isCutSuccessful = (cutSuccessInput == 'Y');

    // Prompt for winning player
    Player* winner = nullptr;
    std::vector<std::string> playerNames;

    Player* currentPlayer = Players.getHead();
    Player* startPlayer = currentPlayer;

    do {
        playerNames.push_back(currentPlayer->getName());
        currentPlayer = currentPlayer->getNext();
    } while (currentPlayer != startPlayer);

    int winningPlayerIndex;
    bool validInput = false;
    do {
        try {
            std::cout << "Select the winning player:\n";
            for (size_t i = 0; i < playerNames.size(); ++i) {
                std::cout << i + 1 << ". " << playerNames[i] << std::endl;
            }
            std::cout << "Please select the winning player: ";
            std::cin >> winningPlayerIndex;

            if (std::cin.fail() || winningPlayerIndex <= 0 || winningPlayerIndex > static_cast<int>(playerNames.size())) {
                throw InvalidInputException();
            }
            else {
                validInput = true;
            }
        }
        catch (const InvalidInputException& e) {
            std::cout << e.what() << " Please enter a valid number.\n";
            clearInputBuffer();
        }
    } while (!validInput);

    // Go to specified player in linked list
    currentPlayer = Players.getHead();
    for (int i = 1; i < winningPlayerIndex; ++i) {
        currentPlayer = currentPlayer->getNext();
    }
    winner = currentPlayer;

    scoringObj.winner = winner;

    // Prompt for scores of remaining players
    std::vector<int> remainingScores;

    currentPlayer = Players.getHead();
    startPlayer = currentPlayer;

    do {
        if (currentPlayer != winner) {
            int score;
            bool validScore = false;
            do {
                try {
                    std::cout << "Enter score for " << currentPlayer->getName() << ": ";
                    std::string input;
                    std::cin >> input;
                    std::istringstream iss(input);
                    if (iss >> score && iss.eof()) {
                        validScore = true;
                        remainingScores.push_back(score);
                    }
                    else {
                        throw InvalidInputException(); // Throw exception for invalid input
                    }
                }
                catch (const InvalidInputException& e) {
                    std::cout << e.what() << " Please enter a number.\n";
                    clearInputBuffer();
                }
            } while (!validScore);
        }
        currentPlayer = currentPlayer->getNext();
    } while (currentPlayer != startPlayer);

    scoringObj.scores = remainingScores;

    return scoringObj;
}
// Prompt user to save and quit
void CLI::endOfRound(Game& game) {
    char continuePlaying;
    do {
        try {
            std::cout << "Do you want to keep playing? (Y/N): ";
            std::cin >> continuePlaying;

            if (continuePlaying != 'Y' && continuePlaying != 'y' &&
                continuePlaying != 'N' && continuePlaying != 'n') {
                clearInputBuffer(); // Clear invalid input
                throw InvalidInputException();
            }

            clearInputBuffer(); // Clear the input buffer after reading char input
            continuePlaying = std::toupper(continuePlaying);
        }
        catch (const InvalidInputException& e) {
            std::cout << e.what() << " Please enter Y or N.\n";
        }
    } while (continuePlaying != 'Y' && continuePlaying != 'N');

    if (continuePlaying == 'N') {
        char saveGame;
        do {
            try {
                std::cout << "Do you want to save your game? (Y/N): ";
                std::cin >> saveGame;
                clearInputBuffer();
                saveGame = std::toupper(saveGame);

                if (saveGame != 'Y' && saveGame != 'N') {
                    throw InvalidInputException();
                }
            }
            catch (const InvalidInputException& e) {
                std::cout << e.what() << " Please enter Y or N.\n";
            }
        } while (saveGame != 'Y' && saveGame != 'N');

        if (saveGame == 'Y') {
            std::string filename = "myGame.txt";
            if (savedGameExists()) {
                char overwrite;
                do {
                    try {
                        std::cout << "A saved game already exists. Do you want to overwrite it? (Y/N): ";
                        std::cin >> overwrite;
                        clearInputBuffer();
                        overwrite = std::toupper(overwrite);

                        if (overwrite != 'Y' && overwrite != 'N') {
                            throw InvalidInputException();
                        }
                    }
                    catch (const InvalidInputException& e) {
                        std::cout << e.what() << " Please enter Y or N.\n";
                    }
                } while (overwrite != 'Y' && overwrite != 'N');

                if (overwrite == 'N') {
                    std::string newFilename = "myGame_backup.txt";
                    std::rename(filename.c_str(), newFilename.c_str());
                    std::cout << "File already exists. Renamed as: " << newFilename << std::endl;
                }
            }

            std::cout << "Saving game...\n";
            game.saveToFile(filename);
        }

        std::cout << "Quitting the game...\n";
        exit(0);
    }
}
// Prompt user to load existing game or create a new one
char CLI::loadOrCreate() {
    char loadOrCreate;
    bool gameLoaded = savedGameExists();

    if (gameLoaded) {
        do {
            try {
                std::cout << "Would you like to load from a file or create a new game? (L for Load / C for Create): ";
                std::cin >> loadOrCreate;

                if (loadOrCreate != 'L' && loadOrCreate != 'l' &&
                    loadOrCreate != 'C' && loadOrCreate != 'c') {
                    clearInputBuffer(); // Clear invalid input
                    throw InvalidInputException();
                }

                clearInputBuffer(); // Clear the input buffer after reading char input
                loadOrCreate = std::toupper(loadOrCreate);
            }
            catch (const InvalidInputException& e) {
                std::cout << e.what() << " Please enter L or C.\n";
            }
        } while (loadOrCreate != 'L' && loadOrCreate != 'C');
    }
    else {
        std::cout << "No saved game found. Creating a new game." << std::endl;
        loadOrCreate = 'C'; // Force creation of a new game
    }

    return loadOrCreate;
}

// Print each player's score next to their name
void CLI::printScores(PlayerList& Players) {
    Player* currentPlayer = Players.getHead();
    Player* startPlayer = currentPlayer;

    do {
        std::cout << currentPlayer->getName() << ": " << currentPlayer->getScore() << std::endl;
        currentPlayer = currentPlayer->getNext();
    } while (currentPlayer != startPlayer);
}
// Print current round, meld, cutter, dealer, and player scores using printScores()
void CLI::displayGame(Game &gameObj) {
    std::cout << "Current round: " << gameObj.getRound() << std::endl;
    std::cout << "Current meld: " << meld[gameObj.getRound() - 1] << "\n---\n";
    std::cout << "Cutter: " << gameObj.getPlayerList().getCutter()->getName() << "\n";
    std::cout << "Dealer: " << gameObj.getPlayerList().getDealer()->getName() << "\n---\nScores:\n";
    printScores(gameObj.getPlayerList());
    std::cout << "---\n";
};
// Print final scores and congratulate winner. Save final scores to file.
void CLI::displayFinalScore(Game& game) {
    // Save the game before displaying final scores
    std::cout << "Saving final scores...\n";
    game.saveToFile("finalScores.txt");

    std::vector<Player*> players;
    Player* currentPlayer = game.getPlayerList().getHead();
    Player* startPlayer = currentPlayer;

    do {
        players.push_back(currentPlayer);
        currentPlayer = currentPlayer->getNext();
    } while (currentPlayer != startPlayer);

    // Find the player with the lowest score
    Player* lowestScorePlayer = players[0];
    for (const auto& player : players) {
        if (player->getScore() < lowestScorePlayer->getScore()) {
            lowestScorePlayer = player;
        }
    }

    // Congratulate the player with the lowest score
    std::cout << "\nCongratulations, " << lowestScorePlayer->getName() << " wins!\n";

    // Display final scores
    std::cout << "Final Scores:\n";
    for (const auto& player : players) {
        std::cout << player->getName() << ": " << player->getScore() << std::endl;
    }
}
