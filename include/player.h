#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "role.h"
#include "team.h"

class Player {
  private:
    QString name;
    ROLE role;
    TEAM team;
    int wins;
  
  public:
    explicit Player(const QString& name, const ROLE& role, const TEAM& team);
    ~Player();

    void setName(const QString& newName);
    QString getName() const;

    void setRole(const ROLE& newRole);
    ROLE getRole() const;

    void setTeam(const TEAM& team);
    TEAM getTeam() const;

    void setWin(const int& wins);
    int getWin() const;
};

#endif
