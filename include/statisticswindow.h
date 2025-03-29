#ifndef STATISTICS_WINDOW_H
#define STATISTICS_WINDOW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "user.h"

class User;
class StatisticsWindow : public QWidget {
  Q_OBJECT

 signals:
  void backToMainWindow();

 public:
  explicit StatisticsWindow(QWidget* parent = nullptr);
  ~StatisticsWindow();

 public slots:
  void show();

 private:
  User* users;

  QPushButton* backToMainButton;

  QComboBox* usernameComboBox;
  QPushButton* showUserStatsButton;

  QString username;
  QLabel* usernameTitle;

  // QLabel* gamesPlayedTitle;
  QLabel* gamesPlayedStats;

  // QLabel* gamesWinTitle;
  QLabel* gamesWinStats;

  // QLabel* gamesWinRateTitle;
  QLabel* gamesWinRateStats;

  // QLabel* guessTotalTitle;
  QLabel* guessTotalStats;

  // QLabel* guessHitTitle;
  QLabel* guessHitStats;

  // QLabel* guessHitRateTitle;
  QLabel* guessHitRateStats;

 private:
  void populateDropDown();

 private slots:
  void goBackToMain();
  void showUserStats();
};

#endif