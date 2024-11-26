#include "PlayerList.h"

// Construct circular doubly linked list from vector of player names
PlayerList::PlayerList(const std::vector<std::string>& names) : head(nullptr) {
    if (names.empty()) {
        std::cerr << "Error: Empty name array." << std::endl;
        return;
    }

    head = new Player(names[0], nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);

    dealer = head;

    Player* current = head;

    for (size_t i = 1; i < names.size(); ++i) {
        Player* newPlayer = new Player(names[i], nullptr, nullptr);
        newPlayer->setNext(head);
        newPlayer->setPrev(current);
        current->setNext(newPlayer);
        head->setPrev(newPlayer);
        current = newPlayer;
    }

    cutter = current;
}

// Safely destroy linked list
PlayerList::~PlayerList() {
    if (head == nullptr)
        return;

    Player* current = head;
    do {
        Player* temp = current;
        current = current->getNext();
        delete temp;
    } while (current != head);
}

// Getters
Player* PlayerList::getHead() const {
    return head;
}
Player* PlayerList::getDealer() const {
    return dealer;
}
Player* PlayerList::getCutter() const {
    return cutter;
}
Player* PlayerList::getPlayer(const std::string& name) const {
    if (head == nullptr) {
        throw std::runtime_error("Player list is empty.");
    }

    Player* currentPlayer = head;
    do {
        if (currentPlayer->getName() == name) {
            return currentPlayer;
        }
        currentPlayer = currentPlayer->getNext();
    } while (currentPlayer != head);

    throw std::invalid_argument("Player not found: " + name);
}

// Setters
void PlayerList::setCutter(const std::string& playerName) {
    Player* current = head;
    while (current != nullptr && current->getName() != playerName) {
        current = current->getNext();
        if (current == head) break; // Break if we reach the starting point
    }
    if (current != nullptr) {
        cutter = current;
    }
    else {
        std::cerr << "Player not found in the list: " << playerName << std::endl;
    }
}
void PlayerList::setDealer(const std::string& playerName) {
    Player* current = head;
    while (current != nullptr && current->getName() != playerName) {
        current = current->getNext();
        if (current == head) break; // Break if we reach the starting point
    }
    if (current != nullptr) {
        dealer = current;
    }
    else {
        std::cerr << "Player not found in the list: " << playerName << std::endl;
    }
}
void PlayerList::setHead(const std::string& playerName) {
    Player* current = head;
    while (current != nullptr && current->getName() != playerName) {
        current = current->getNext();
        if (current == head) break; // Break if we reach the starting point
    }
    if (current != nullptr) {
        head = current;
    }
    else {
        std::cerr << "Player not found in the list: " << playerName << std::endl;
    }
}

// Modifiers
void PlayerList::nextDealer() {
    if (dealer != nullptr) {
        dealer = dealer->getNext();
    }
}
void PlayerList::nextCutter() {
    if (cutter != nullptr) {
        cutter = cutter->getNext();
    }
}
void PlayerList::nextHead() {
    if (head != nullptr) {
        head = head->getNext();
    }
}