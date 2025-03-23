#include "player.h"

// Constructor
Player::Player(const QString& name, const ROLE& role, const TEAM& team)
    : name(name), role(role), team(team), wins(0) {}  // Initialize wins to 0

// No dynamic memory, default destructor is sufficient
Player::~Player() = default;

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
