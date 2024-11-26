#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>

class Player {

public:
	Player(const std::string&, Player*, Player*);
	~Player();

	// Getters
	std::string getName() const;
	int getScore() const;
	Player* getNext() const;
	Player* getPrev() const;

	// Setters
	void setNext(Player*);
	void setPrev(Player*);
	void setScore(int);

	// Modifiers
	void addPoints(int);
	void subtractPoints(int);

private:
	std::string name;
	int score;
	Player* next; //pointer to player sitting to the left
	Player* prev; //pointer to player sitting to the right

};

#endif