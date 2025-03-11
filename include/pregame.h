#ifndef PREGAME_H
#define PREGAME_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "localstart.h"

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

 private slots:
  void goBackToMain();  // Slot to handle back button click
  void startGame();

 signals:
  void backToMainWindow();  // Signal to notify MainWindow to show itself
  void start();

 private:
  QLabel* label;
  QPushButton* backButton;
  QPushButton* startButton;

  QLineEdit* redTeamSpyMasterNickname;
  QLineEdit* redTeamOperativeNickname;
  QLineEdit* blueTeamSpyMasterNickname;
  QLineEdit* blueTeamOperativeNickname;

  QVBoxLayout* layout;
  QHBoxLayout* teamsLayout;
  QVBoxLayout* redTeamLayout;
  QVBoxLayout* blueTeamLayout;
  QHBoxLayout* buttonsLayout;
};

#endif  // PREGAME_H