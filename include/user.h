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

#include <QComboBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QWidget>

#include "createaccountwindow.h"

class CreateAccountWindow;

/**
 * @brief User class to handle local log in and loading/storing json files.
 * This is a singleton class
 *
 */
class User : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Getting the instance of user
   *
   * @param parent parent of the User class
   * @return `User*` User instance
   */
  static User* instance(QWidget* parent = nullptr);

  /**
   * @brief Destructor of user class
   *
   */
  ~User();

  /**
   * @brief Update the number of games played by a user
   *
   * @param username username of the user
   * @param newGamesPlayed the new number of games played by a user
   */
  void updateGamesPlayed(const QString& username,
                         const unsigned int& newGamesPlayed);

  /**
   * @brief Get the number of games played by a user
   *
   * @param username username of the user
   * @return `unsigned int` the number of games played
   */
  unsigned int getGamesPlayed(const QString& username) const;

  /**
   * @brief Update the number of wins a user has
   *
   * @param username username of the user
   * @param newWins the new number of wins the user has
   */
  void updateWins(const QString& username, const unsigned int& newWins);

  /**
   * @brief Get the number of wins the user has
   *
   * @param username username of the user
   * @return `unsigned int` the number of wins the user has
   */
  unsigned int getWins(const QString& username) const;

  /**
   * @brief Get the win rate of the user (games_win/games_played)
   *
   * @param username the username of a user
   * @return `float` win rate of the user (games_win/games_played)
   */
  float getWinRate(const QString& username) const;

  /**
   * @brief Update the total of guesses the user has
   *
   * @param username username of the user
   * @param newGuessTotal the new total number of guesses the user has
   */
  void updateGuessTotal(const QString& username,
                        const unsigned int& newGuessTotal);

  /**
   * @brief Get the total number of guesses the user has
   *
   * @param username username of the user
   * @return `unsigned int` the total number of guesses the user has
   */
  unsigned int getGuessTotal(const QString& username) const;

  /**
   * @brief Update the number times the user guess correctly
   *
   * @param username username of the user
   * @param newGuessHit the number of times the user guess correctly
   */
  void updateGuessHit(const QString& username, const unsigned int& newGuessHit);

  /**
   * @brief Get the number of times the user guess correctly
   *
   * @param username username of the user
   * @return `unsigned int` the number of times the user guess correctly
   */
  unsigned int getGuessHit(const QString& username) const;

  /**
   * @brief Get the rate the user guess correctly (guess_hit/guess_total)
   *
   * @param username username of the user
   * @return `float` the rate the user guess correctly (guess_hit/guess_total)
   */
  float getHitRate(const QString& username);

  /**
   * @brief Rename the user
   *
   * @param oldUsername old username of the user
   * @param newUsername new username of the user
   */
  void renameUser(const QString& oldUsername, const QString& newUsername);

  /**
   * @brief Change the games played total and games played win of the user when
   * they won
   *
   * @param username username of the user
   */
  void won(const QString& username);

  /**
   * @brief Change the games played total of the user when they lost
   *
   * @param username username of the user
   */
  void lost(const QString& username);

  /**
   * @brief Change the guess total and guess hit of the user when they guess
   * correctly
   *
   * @param username username of the user
   */
  void hit(const QString& username);

  /**
   * @brief Change the guess total of the user when they guess incorrectly
   *
   * @param username username of the user
   */
  void miss(const QString& username);

  /**
   * @brief loading the info of the users
   *
   * @return `QJsonObject` the info of the user in json format
   */
  QJsonObject loadJsonFile();  // Function to load JSON data

 public slots:
  /**
   * @brief show the current screen
   *
   */
  void show();

 signals:
  /**
   * @brief signal to go to main menu
   *
   */
  void backToMainMenu();

 private slots:
  /**
   * @brief log in the user
   *
   */
  void handleLogin();

  /**
   * @brief refresh user info in the drop down menu
   *
   */
  void refreshUserDropdown();

  /**
   * @brief create user account
   *
   */
  void handleCreateAccount();

  /**
   * @brief show the main menu
   *
   */
  void showMainMenu();

 private:
  /**
   * @brief Constructor of the User instance
   *
   * @param parent the parent of User class
   */
  explicit User(QWidget* parent = nullptr);

  /**
   * @brief variable that stores the create account window
   *
   */
  CreateAccountWindow* createAccountWindow;

  /**
   * @brief the path of the users info
   *
   */
  QString jsonFilePath = "resources/profile.json";

  /**
   * @brief the button to go back
   *
   */
  QPushButton* backButton;

  /**
   * @brief the button to create account
   *
   */
  QPushButton* createAccountButton;

  /**
   * @brief the drop down box of the usernames of the users
   *
   */
  QComboBox* usernameComboBox;

  /**
   * @brief the text to show the debug
   *
   */
  QLabel* jsonContentLabel;

  /**
   * @brief the button to log in
   *
   */
  QPushButton* loginButton;

  /**
   * @brief update the usernames in the drop down when creating new users
   *
   * @param jsonObject the json of the users
   */
  void populateUsernameComboBox(const QJsonObject& jsonObject);
};

#endif  // USER_H
