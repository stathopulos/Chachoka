#include "Player.h"

Player::Player(const std::string& playerName, Player* nextPlayer, Player* prevPlayer) : name(playerName), score(0), next(nextPlayer), prev(prevPlayer) {}

Player::~Player() {}

// Setters
void Player::setNext(Player* player) {
	next = player;
}
void Player::setPrev(Player* player) {
	prev = player;
}
void Player::setScore(int newScore) {
	score = newScore;
}

// Getters
std::string Player::getName() const {
	return name;
}
int Player::getScore() const {
	return score;
}
Player* Player::getNext() const {
	return next;
}
Player* Player::getPrev() const {
	return next;
}

// Modifiers
void Player::addPoints(int points) {
	score += points;
}
void Player::subtractPoints(int points) {
	score -= points;
}