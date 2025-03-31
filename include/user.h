/**
 * @file user.h
 * @author Team 9 - UWO CS 3307
 * @brief User class to handle local log in and loading/storing json files
 * @version 0.1
 * @date 2025-03-30
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef USER_H
#define USER_H

// Qt framework includes for UI components and file handling
#include <QComboBox>           // For dropdown menu of usernames
#include <QCryptographicHash>  // For password hashing functionality
#include <QDebug>              // For debug output to console
#include <QDir>                // For directory manipulation
#include <QFile>               // For file I/O operations
#include <QJsonDocument>       // For JSON document parsing
#include <QJsonObject>         // For JSON object manipulation
#include <QLabel>              // For text display in UI
#include <QLineEdit>           // For text input fields
#include <QPushButton>         // For button UI elements
#include <QStandardPaths>      // For accessing standard file locations
#include <QVBoxLayout>         // For vertical layout arrangement
#include <QWidget>             // Base class for all UI elements

#include "createaccountwindow.h"  // Include for account creation UI

// Forward declaration to resolve circular dependency
class CreateAccountWindow;

/**
 * @brief User class to handle local log in and loading/storing json files.
 * This is a singleton class to ensure only one instance of user management
 * exists. Manages user profiles, statistics, and authentication.
 */
class User : public QWidget {
 Q_OBJECT  // Qt macro for enabling signals and slots mechanism

     public :
     /**
      * @brief Getting the instance of user (Singleton pattern implementation)
      * Ensures only one instance of User class exists throughout the
      * application
      *
      * @param parent parent QWidget for ownership hierarchy
      * @return *`User` Pointer to the single User instance
      */
     static User*
     instance(QWidget* parent = nullptr);

  /**
   * @brief Destructor of user class
   * Cleans up resources when User object is destroyed
   */
  ~User();

  /**
   * @brief Update the number of games played by a user
   * Modifies user statistics and saves to profile
   *
   * @param username username of the user to update
   * @param newGamesPlayed the new number of games played by a user
   */
  void updateGamesPlayed(const QString& username,
                         const unsigned int& newGamesPlayed);

  /**
   * @brief Get the number of games played by a user
   * Retrieves game count from user profile
   *
   * @param username username of the user
   * @return `unsigned int` the number of games played
   */
  unsigned int getGamesPlayed(const QString& username) const;

  /**
   * @brief Update the number of wins a user has
   * Modifies win statistics and saves to profile
   *
   * @param username username of the user
   * @param newWins the new number of wins the user has
   */
  void updateWins(const QString& username, const unsigned int& newWins);

  /**
   * @brief Get the number of wins the user has
   * Retrieves win count from user profile
   *
   * @param username username of the user
   * @return `unsigned int` the number of wins the user has
   */
  unsigned int getWins(const QString& username) const;

  /**
   * @brief Get the win rate of the user (games_win/games_played)
   * Calculates win percentage based on games played and won
   *
   * @param username the username of a user
   * @return `float` win rate of the user (games_win/games_played)
   */
  float getWinRate(const QString& username) const;

  /**
   * @brief Update the total of guesses the user has
   * Modifies guess statistics and saves to profile
   *
   * @param username username of the user
   * @param newGuessTotal the new total number of guesses the user has
   */
  void updateGuessTotal(const QString& username,
                        const unsigned int& newGuessTotal);

  /**
   * @brief Get the total number of guesses the user has
   * Retrieves guess count from user profile
   *
   * @param username username of the user
   * @return `unsigned int` the total number of guesses the user has
   */
  unsigned int getGuessTotal(const QString& username) const;

  /**
   * @brief Update the number times the user guess correctly
   * Modifies correct guess statistics and saves to profile
   *
   * @param username username of the user
   * @param newGuessHit the number of times the user guess correctly
   */
  void updateGuessHit(const QString& username, const unsigned int& newGuessHit);

  /**
   * @brief Get the number of times the user guess correctly
   * Retrieves correct guess count from user profile
   *
   * @param username username of the user
   * @return `unsigned int` the number of times the user guess correctly
   */
  unsigned int getGuessHit(const QString& username) const;

  /**
   * @brief Get the rate the user guess correctly (guess_hit/guess_total)
   * Calculates accuracy percentage based on total guesses and correct guesses
   *
   * @param username username of the user
   * @return `float` the rate the user guess correctly (guess_hit/guess_total)
   */
  float getHitRate(const QString& username);

  /**
   * @brief Rename the user
   * Changes username in profile while preserving statistics
   *
   * @param oldUsername old username of the user
   * @param newUsername new username of the user
   */
  void renameUser(const QString& oldUsername, const QString& newUsername);

  /**
   * @brief Change the games played total and games played win of the user when
   * they won
   * Convenience method to update multiple statistics after a win
   *
   * @param username username of the user
   */
  void won(const QString& username);

  /**
   * @brief Change the games played total of the user when they lost
   * Convenience method to update statistics after a loss
   *
   * @param username username of the user
   */
  void lost(const QString& username);

  /**
   * @brief Change the guess total and guess hit of the user when they guess
   * correctly
   * Convenience method to update multiple statistics after a correct guess
   *
   * @param username username of the user
   */
  void hit(const QString& username);

  /**
   * @brief Change the guess total of the user when they guess incorrectly
   * Convenience method to update statistics after an incorrect guess
   *
   * @param username username of the user
   */
  void miss(const QString& username);

  /**
   * @brief loading the info of the users
   * Reads user profiles from JSON storage
   *
   * @return `QJsonObject` the info of the user in json format
   */
  QJsonObject loadJsonFile();  // Function to load JSON data

 public slots:
  /**
   * @brief show the current screen
   * Makes the user login UI visible
   */
  void show();

 signals:
  /**
   * @brief signal to go to main menu
   * Emitted when user successfully logs in or cancels login
   */
  void backToMainMenu();

 private slots:
  /**
   * @brief log in the user
   * Handles authentication and session creation
   */
  void handleLogin();

  /**
   * @brief refresh user info in the drop down menu
   * Updates UI with latest user list
   */
  void refreshUserDropdown();

  /**
   * @brief create user account
   * Opens account creation window
   */
  void handleCreateAccount();

  /**
   * @brief show the main menu
   * Returns to main application screen
   */
  void showMainMenu();

 private:
  /**
   * @brief Constructor of the User instance
   * Private to enforce singleton pattern
   *
   * @param parent the parent QWidget for memory management
   */
  explicit User(QWidget* parent = nullptr);

  /**
   * @brief variable that stores the create account window
   * Manages account creation UI
   */
  CreateAccountWindow* createAccountWindow;

  /**
   * @brief the path of the users info
   * Location of JSON profile storage
   */
  QString jsonFilePath = "resources/profile.json";

  /**
   * @brief the button to go back
   * UI element for navigation
   */
  QPushButton* backButton;

  /**
   * @brief the button to create account
   * UI element to open account creation
   */
  QPushButton* createAccountButton;

  /**
   * @brief the drop down box of the usernames of the users
   * UI element for user selection
   */
  QComboBox* usernameComboBox;

  /**
   * @brief the text to show the debug
   * UI element for displaying information
   */
  QLabel* jsonContentLabel;

  /**
   * @brief the button to log in
   * UI element for authentication
   */
  QPushButton* loginButton;

  /**
   * @brief update the usernames in the drop down when creating new users
   * Refreshes UI with current user list
   *
   * @param jsonObject the json of the users
   */
  void populateUsernameComboBox(const QJsonObject& jsonObject);
};

#endif  // USER_H