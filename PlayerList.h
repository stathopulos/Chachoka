#ifndef PLAYERLIST_H
#define PLAYERLIST_H

#include <vector>
#include <iostream>
#include "Player.h"

class PlayerList {
private:
    Player* head;
    Player* dealer;
    Player* cutter;

public:
    PlayerList(const std::vector<std::string>& names);
    ~PlayerList();

    // Getters
    Player* getHead() const;
    Player* getDealer() const;
    Player* getCutter() const;
    Player* getPlayer(const std::string&) const;

    // Setters
    void setCutter(const std::string& playerName);
    void setDealer(const std::string& playerName);
    void setHead(const std::string& playerName);

    // Modifiers
    void nextDealer();
    void nextCutter();
    void nextHead();
};

#endif // PLAYERLIST_H
