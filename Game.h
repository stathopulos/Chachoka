#ifndef GAME_H
#define GAME_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "PlayerList.h"

// All information necessary to score a round
struct ScoringObj {
    Player* winner;
    bool isCutSuccessful;
    std::vector<int> scores;
};
// All necessary information to completely construct a game
struct GameParams {
    std::vector<std::string> names;
    std::vector<int> scores;
    int roundNum;
    std::string cutterName;
    std::string dealerName;
    std::string headName;
};

class Game {
private:
    PlayerList Players;
    int round;

public:
    Game();
    Game(const std::vector<std::string>&);
    Game(const GameParams& params);
    ~Game();

    // Getters
    int getRound() const;
    PlayerList& getPlayerList();

    // Modifiers
    void nextRound();
    void scoreRound(ScoringObj);

    // File save
    void saveToFile(const std::string&) const;
};

#endif // GAME_H