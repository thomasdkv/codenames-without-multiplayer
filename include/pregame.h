/**
 * @file pregame.h
 * @author Team 9 - UWO CS 3307
 * @brief Header file for the PreGame class which handles the game setup screen
 * @version 0.1
 * @date 2025-03-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef PREGAME_H
#define PREGAME_H

#include <QComboBox>
#include <QDebug>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>

#include "createaccountwindow.h"
#include "gameboard.h"
#include "user.h"

class User;
class CreateAccountWindow;

/**
 * @brief The PreGame class provides the interface for setting up a new game
 *        This includes selecting players for each team and role before starting
 *        the game
 */
class PreGame : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Pre Game object
   *
   * @param parent Optional parent widget for memory management purposes
   */
  explicit PreGame(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Pre Game object and clean up resources
   *
   */
  ~PreGame();

  /**
   * @brief Get the Red Team Spy Master Nickname
   *
   * @return QString The nickname of the selected Red Team Spy Master
   */
  QString getRedTeamSpyMasterNickname() const;

  /**
   * @brief Get the Red Team Operative Nickname
   *
   * @return QString The nickname of the selected Red Team Operative
   */
  QString getRedTeamOperativeNickname() const;

  /**
   * @brief Get the Blue Team Spy Master Nickname
   *
   * @return QString The nickname of the selected Blue Team Spy Master
   */
  QString getBlueTeamSpyMasterNickname() const;

  /**
   * @brief Get the Blue Team Operative Nickname
   *
   * @return QString The nickname of the selected Blue Team Operative
   */
  QString getBlueTeamOperativeNickname() const;

 public slots:
  /**
   * @brief Shows the pregame setup window and initializes user dropdowns
   *
   */
  void show();

 private:
  /**
   * @brief Populates the user selection dropdown menus with available users
   *        This is called when the window is shown to ensure the latest user
   * list
   *
   */
  void populateUserDropdowns();

 private slots:
  /**
   * @brief Returns to the main menu screen
   *        Connected to the back button's clicked signal
   *
   */
  void goBackToMain();

  /**
   * @brief Starts the game with the selected players
   *        Validates player selections and emits start signal if valid
   *
   */
  void startGame();

  /**
   * @brief Handles cleanup after a game has ended
   *        Prepares the UI for a potential new game
   *
   */
  void handleGameEnd();

  /**
   * @brief Opens the account creation window
   *        Connected to the create account button's clicked signal
   *
   */
  void openCreateAccount();

 signals:
  /**
   * @brief Signal emitted when user wants to return to main window
   *        Connected to main window to show it again
   *
   */
  void backToMainWindow();

  /**
   * @brief Signal emitted when all players are selected and game is ready to
   * start Connected to game initialization in the game controller
   *
   */
  void start();

  /**
   * @brief Signal emitted when user list needs to be refreshed
   *        This happens after a new account is created
   *
   */
  void update();

 private:
  /**
   * @brief Pointer to User objects containing player information
   *        Used to populate the dropdown menus
   *
   */
  User* users;

  /**
   * @brief List of available usernames for player selection
   *        Populated from the users database
   *
   */
  QStringList usernames;

  /**
   * @brief Pointer to the account creation window
   *        Initialized when create account button is clicked
   *
   */
  CreateAccountWindow* createAccountWindow;

  /**
   * @brief Title label for the pregame screen
   *
   */
  QLabel* label;

  /**
   * @brief Button to return to the main menu
   *
   */
  QPushButton* backButton;

  /**
   * @brief Button to open the account creation window
   *
   */
  QPushButton* createAccountButton;

  /**
   * @brief Button to start the game with selected players
   *
   */
  QPushButton* startButton;

  /**
   * @brief Dropdown menu for selecting the Red Team's Spy Master
   *
   */
  QComboBox* redTeamSpyMasterComboBox;

  /**
   * @brief Dropdown menu for selecting the Red Team's Operative
   *
   */
  QComboBox* redTeamOperativeComboBox;

  /**
   * @brief Dropdown menu for selecting the Blue Team's Spy Master
   *
   */
  QComboBox* blueTeamSpyMasterComboBox;

  /**
   * @brief Dropdown menu for selecting the Blue Team's Operative
   *
   */
  QComboBox* blueTeamOperativeComboBox;

  /**
   * @brief Main vertical layout for the entire pregame screen
   *
   */
  QVBoxLayout* layout;

  /**
   * @brief Horizontal layout to contain both team selection areas
   *
   */
  QHBoxLayout* teamsLayout;

  /**
   * @brief Vertical layout for the Red Team's player selections
   *
   */
  QVBoxLayout* redTeamLayout;

  /**
   * @brief Vertical layout for the Blue Team's player selections
   *
   */
  QVBoxLayout* blueTeamLayout;

  /**
   * @brief Horizontal layout for the navigation buttons
   *
   */
  QHBoxLayout* buttonsLayout;

  /**
   * @brief Pointer to the game board that will be shown after game starts
   *
   */
  GameBoard* gameBoard;
};

#endif  // PREGAME_H