/**
 * @file statisticswindow.h
 * @author Team 9 - UWO CS 3307
 * @brief The screen to show the user's statistics
 * @version 0.1
 * @date 2025-03-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef STATISTICS_WINDOW_H
#define STATISTICS_WINDOW_H

#include <QComboBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QString>
#include <QVBoxLayout>

#include "user.h"

class User;

/**
 * @brief The class that shows the Statistics screen
 *
 */
class StatisticsWindow : public QWidget {
  Q_OBJECT

 signals:
  /**
   * @brief Go back to the main window
   *
   */
  void backToMainWindow();

 public:
  /**
   * @brief Construct a new Statistics Window object
   *
   * @param parent the parent of the statistics window screen
   */
  explicit StatisticsWindow(QWidget* parent = nullptr);

  /**
   * @brief Destructor for statistics screen
   *
   */
  ~StatisticsWindow();

 public slots:
  /**
   * @brief show the statistics screen
   *
   */
  void show();

 private:
  /**
   * @brief the users instance
   *
   */
  User* users;

  /**
   * @brief button to click to go back to main
   *
   */
  QPushButton* backToMainButton;

  /**
   * @brief the drop down box of usernames
   *
   */
  QComboBox* usernameComboBox;

  /**
   * @brief the button to show the user stats after choosing in drop down menu
   *
   */
  QPushButton* showUserStatsButton;

  /**
   * @brief the username of the user
   *
   */
  QString username;

  /**
   * @brief title of username
   *
   */
  QLabel* usernameTitle;

  /**
   * @brief the number of games played of the user
   *
   */
  QLabel* gamesPlayedStats;

  /**
   * @brief the number of games win of the user
   *
   */
  QLabel* gamesWinStats;

  /**
   * @brief the win rate of the user
   *
   */
  QLabel* gamesWinRateStats;

  /**
   * @brief the number of guess total of the user
   *
   */
  QLabel* guessTotalStats;

  /**
   * @brief the number of correct guess of the user
   *
   */
  QLabel* guessHitStats;

  /**
   * @brief the guess hit rate of the user
   *
   */
  QLabel* guessHitRateStats;

 private:
  /**
   * @brief populate the drop down button with the usernames
   *
   */
  void populateDropDown();

 private slots:
  /**
   * @brief to back to the main window
   *
   */
  void goBackToMain();

  /**
   * @brief showing the user stats after clicking the button
   *
   */
  void showUserStats();
};

#endif