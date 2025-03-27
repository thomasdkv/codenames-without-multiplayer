#ifndef PREGAME_H
#define PREGAME_H

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>

#include "game.h"
#include "gameboard.h"
#include "user.h"
#include "createaccountwindow.h"

class CreateAccountWindow;
class PreGame : public QWidget {
  Q_OBJECT

 public:
  explicit PreGame(QWidget* parent = nullptr);
  ~PreGame();

  // Getter functions to get the text entered in the textboxes
  QString getRedTeamSpyMasterNickname() const;
  QString getRedTeamOperativeNickname() const;
  QString getBlueTeamSpyMasterNickname() const;
  QString getBlueTeamOperativeNickname() const;

 private:
  void populateUserDropdowns();

 private slots:
  void goBackToMain();  // Slot to handle back button click
  void startGame();
  void showPreGame();
  void handleGameEnd();
  void openCreateAccount();

 signals:
  void backToMainWindow();  // Signal to notify MainWindow to show itself
  void start();

 private:
  Game* game;

  User *users;

  CreateAccountWindow* createAccountWindow;

  QLabel* label;
  QPushButton* backButton;
  QPushButton* createAccountButton;
  QPushButton* startButton;

  // QLineEdit* redTeamSpyMasterNickname;
  // QLineEdit* redTeamOperativeNickname;
  // QLineEdit* blueTeamSpyMasterNickname;
  // QLineEdit* blueTeamOperativeNickname;

  QComboBox* redTeamSpyMasterComboBox;
  QComboBox* redTeamOperativeComboBox;
  QComboBox* blueTeamSpyMasterComboBox;
  QComboBox* blueTeamOperativeComboBox;

  QVBoxLayout* layout;
  QHBoxLayout* teamsLayout;
  QVBoxLayout* redTeamLayout;
  QVBoxLayout* blueTeamLayout;
  QHBoxLayout* buttonsLayout;

  GameBoard* gameBoard;
};

#endif  // PREGAME_H