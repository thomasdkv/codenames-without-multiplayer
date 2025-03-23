#include "player.h"

// Constructor
Player::Player(const QString& name, const ROLE& role, const TEAM& team)
    : name(name), role(role), team(team), wins(0) {}  // Initialize wins to 0

Player::~Player() {};

// Setters
void Player::setName(const QString& newName) {
    name = newName;
}

void Player::setRole(const ROLE& newRole) {
    role = newRole;
}

void Player::setTeam(const TEAM& newTeam) {
    team = newTeam;
}

void Player::setWin(const int& wins) {
  this->wins = wins;
}

// Getters
QString Player::getName() const {
    return name;
}

ROLE Player::getRole() const {
    return role;
}

TEAM Player::getTeam() const {
    return team;
}

int Player::getWin() const {
  return wins;
}
