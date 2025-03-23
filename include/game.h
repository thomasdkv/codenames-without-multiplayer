#ifndef GAME_H
#define GAME_H

#include <QList>

#include "player.h"

class Game {
 private:
  QList<Player> players;
  QList<Player> teamRed;
  QList<Player> teamBlue;
  QList<Player> spyMasters;
  QList<Player> fieldOperatives;

 public:
  // Constructor
  explicit Game();

  // Getters
  QList<Player> getPlayers() const;
  QList<Player> getTeamRed() const;
  QList<Player> getTeamBlue() const;
  QList<Player> getSpyMasters() const;
  QList<Player> getFieldOperatives() const;

  // Setters
  void setPlayers(const QList<Player>& newPlayers);
  void setTeamRed(const QList<Player>& newTeamRed);
  void setTeamBlue(const QList<Player>& newTeamBlue);
  void setSpyMasters(const QList<Player>& newSpyMasters);
  void setFieldOperatives(const QList<Player>& newFieldOperatives);

  // Add player and categorize them into respective lists
  void addPlayer(const Player& player);
};

#endif
