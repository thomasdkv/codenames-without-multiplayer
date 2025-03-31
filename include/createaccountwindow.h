/**
 * @file createaccountwindow.h
 * @author Team 9 - UWO CS 3307
 * @brief Header file for the CreateAccountWindow class which handles user
 * account creation
 * @version 0.1
 * @date 2025-03-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef CREATEACCOUNTWINDOW_H
#define CREATEACCOUNTWINDOW_H

#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

/**
 * @brief The CreateAccountWindow class provides a singleton interface for
 * creating new user accounts This window allows users to input a username and
 * creates a profile JSON file for the new account
 */
class CreateAccountWindow : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Get the singleton instance of CreateAccountWindow
   *        Creates the instance if it doesn't exist yet
   *
   * @param parent Optional parent widget for memory management purposes
   * @return CreateAccountWindow* Pointer to the singleton instance
   */
  static CreateAccountWindow* getInstance(QWidget* parent = nullptr);

  /**
   * @brief Set the previous screen to return to when operation is complete
   *        Used for navigation back to the calling screen
   *
   * @param previous Pointer to the widget to return to
   */
  void setPreviousScreen(QWidget* previous);

 public slots:
  /**
   * @brief Displays the account creation window and prepares the UI
   *        Resets status messages and input fields when shown
   */
  void show();

 private:
  /**
   * @brief Private constructor to enforce singleton pattern
   *        Initializes UI components for account creation
   *
   * @param parent Optional parent widget for memory management
   */
  explicit CreateAccountWindow(QWidget* parent = nullptr);

  /**
   * @brief Static pointer to the singleton instance
   *        Ensures only one instance exists throughout the application
   */
  static CreateAccountWindow* instance;

 private slots:
  /**
   * @brief Handles the create account button click event
   *        Validates input and creates a new user profile if valid
   */
  void onCreateAccountClicked();

  /**
   * @brief Returns to the previous screen
   *        Called when account creation is complete or canceled
   */
  void goBack();

 signals:
  /**
   * @brief Signal emitted when returning to the previous screen
   *        Connected to the appropriate handler in the previous screen
   */
  void back();

  /**
   * @brief Signal emitted when a new account is successfully created
   *        Notifies other components to update their user lists
   */
  void accountCreated();

 private:
  /**
   * @brief Creates and saves a JSON profile file for the new user
   *        Stores basic user information in the specified JSON file
   *
   * @param username The username for the new account
   */
  void saveJsonFile(const QString& username);

  /**
   * @brief Text input field for entering the new username
   */
  QLineEdit* usernameEdit;

  /**
   * @brief Button to submit account creation request
   */
  QPushButton* createAccountButton;

  /**
   * @brief Label to display status messages and error feedback
   */
  QLabel* statusLabel;

  /**
   * @brief Path to the JSON profile file where user data will be stored
   *        May need to be updated based on deployment environment
   */
  QString jsonFilePath = "resources/profile.json";  // Update path as necessary

  /**
   * @brief Pointer to the previous screen to return to after account creation
   *        Set via setPreviousScreen() method
   */
  QWidget* previousScreen = nullptr;
};

#endif  // CREATEACCOUNTWINDOW_H