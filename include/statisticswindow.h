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

// Qt framework includes for UI components and screen management
#include <QComboBox>        // For dropdown menu of usernames
#include <QGuiApplication>  // For application-level GUI functionality
#include <QHBoxLayout>      // For horizontal layout arrangement
#include <QLabel>           // For text display in UI
#include <QPushButton>      // For button UI elements
#include <QScreen>          // For screen geometry information
#include <QString>          // For string handling
#include <QVBoxLayout>      // For vertical layout arrangement

#include "user.h"  // Include for user data access

// Forward declaration to resolve circular dependency
class User;

/**
 * @brief The class that shows the Statistics screen
 * Displays game statistics for selected users including win rates and guess
 * accuracy
 */
class StatisticsWindow : public QWidget {
  Q_OBJECT  // Qt macro for enabling signals and slots mechanism

      signals :
      /**
       * @brief Go back to the main window
       * Signal emitted when user chooses to return to main menu
       */
      void
      backToMainWindow();

 public:
  /**
   * @brief Construct a new Statistics Window object
   * Initializes UI components and connects signals/slots
   *
   * @param parent the parent of the statistics window screen for widget
   * hierarchy
   */
  explicit StatisticsWindow(QWidget* parent = nullptr);

  /**
   * @brief Destructor for statistics screen
   * Cleans up resources when StatisticsWindow is destroyed
   */
  ~StatisticsWindow();

 public slots:
  /**
   * @brief show the statistics screen
   * Makes the statistics UI visible and updates data
   */
  void show();

 private:
  /**
   * @brief the users instance
   * Singleton reference to access user data and statistics
   */
  User* users;

  /**
   * @brief button to click to go back to main
   * UI navigation element to return to main menu
   */
  QPushButton* backToMainButton;

  /**
   * @brief the drop down box of usernames
   * Selection widget for choosing which user's statistics to display
   */
  QComboBox* usernameComboBox;

  /**
   * @brief the button to show the user stats after choosing in drop down menu
   * Triggers update of statistics display for selected user
   */
  QPushButton* showUserStatsButton;

  /**
   * @brief the username of the user
   * Stores the currently selected username
   */
  QString username;

  /**
   * @brief title of username
   * Display label showing selected user's name
   */
  QLabel* usernameTitle;

  /**
   * @brief the number of games played of the user
   * Display label showing total games played statistic
   */
  QLabel* gamesPlayedStats;

  /**
   * @brief the number of games win of the user
   * Display label showing total games won statistic
   */
  QLabel* gamesWinStats;

  /**
   * @brief the win rate of the user
   * Display label showing win percentage (wins/games played)
   */
  QLabel* gamesWinRateStats;

  /**
   * @brief the number of guess total of the user
   * Display label showing total guesses made statistic
   */
  QLabel* guessTotalStats;

  /**
   * @brief the number of correct guess of the user
   * Display label showing correct guesses statistic
   */
  QLabel* guessHitStats;

  /**
   * @brief the guess hit rate of the user
   * Display label showing guess accuracy percentage (hits/total)
   */
  QLabel* guessHitRateStats;

 private:
  /**
   * @brief populate the drop down button with the usernames
   * Fetches user list from User singleton and fills dropdown menu
   */
  void populateDropDown();

 private slots:
  /**
   * @brief to back to the main window
   * Slot triggered when back button is clicked
   */
  void goBackToMain();

  /**
   * @brief showing the user stats after clicking the button
   * Slot that retrieves and displays statistics for selected user
   */
  void showUserStats();
};

#endif  // STATISTICS_WINDOW_H