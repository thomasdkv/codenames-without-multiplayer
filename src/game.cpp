#include "game.h"

Game::Game() {}

// Getters
QList<Player> Game::getPlayers() const { return players; }

QList<Player> Game::getTeamRed() const { return teamRed; }

QList<Player> Game::getTeamBlue() const { return teamBlue; }

QList<Player> Game::getSpyMasters() const { return spyMasters; }

QList<Player> Game::getFieldOperatives() const { return fieldOperatives; }

// Setters
void Game::setPlayers(const QList<Player>& newPlayers) { players = newPlayers; }

void Game::setTeamRed(const QList<Player>& newTeamRed) { teamRed = newTeamRed; }

void Game::setTeamBlue(const QList<Player>& newTeamBlue) {
  teamBlue = newTeamBlue;
}

void Game::setSpyMasters(const QList<Player>& newSpyMasters) {
  spyMasters = newSpyMasters;
}

void Game::setFieldOperatives(const QList<Player>& newFieldOperatives) {
  fieldOperatives = newFieldOperatives;
}

// Function to add a player and classify them
void Game::addPlayer(const Player& player) {
  players.append(player);

  // Categorize by team
  if (player.getTeam() == TEAM::RED) {
    teamRed.append(player);
  } else if (player.getTeam() == TEAM::BLUE) {
    teamBlue.append(player);
  }

  // Categorize by role
  if (player.getRole() == ROLE::SPYMASTER) {
    spyMasters.append(player);
  } else if (player.getRole() == ROLE::OPERATIVE) {
    fieldOperatives.append(player);
  }
}
