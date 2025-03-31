/**
 * @file createaccountwindow.cpp
 * @author Team 9 - UWO CS 3307
 * @brief CPP file for the CreateAccountWindow class which handles user
 * account creation
 * @version 0.1
 * @date 2025-03-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "createaccountwindow.h"

CreateAccountWindow* CreateAccountWindow::instance = nullptr;

CreateAccountWindow* CreateAccountWindow::getInstance(QWidget* parent) {
  if (instance == nullptr) {
    instance = new CreateAccountWindow(parent);
  }
  return instance;
}

CreateAccountWindow::CreateAccountWindow(QWidget* parent) : QWidget(parent) {
  // Button Styling
  QString buttonStyles =
      "QPushButton {"
      "   background-color: rgb(65, 42, 213);"
      "   color: white;"
      "   border-radius: 5px;"
      "   border: 2px solid #412AD5;"
      "   padding: 5px;"
      "   font-weight: bold;"
      "   font-size: 20px;"
      "}"
      "QPushButton:hover {"
      "   background-color: rgb(54, 35, 177);"  // Hover background color
      "}";

  // Set up the window
  setWindowTitle("Create Account");
  this->setFixedSize(1000, 800);

  // Create title label
  QLabel* titleLabel = new QLabel("Create Account", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet("font-weight: bold; font-size: 24px;");
  titleLabel->move(415, 300);

  // Create username input field
  usernameEdit = new QLineEdit(this);
  usernameEdit->setFixedSize(400, 30);
  usernameEdit->move(300, 350);
  usernameEdit->setPlaceholderText("Enter username");

  // Create the "Create Account" button
  createAccountButton = new QPushButton("Create", this);
  createAccountButton->setFixedSize(200, 50);
  createAccountButton->move(400, 400);
  createAccountButton->setStyleSheet(buttonStyles);

  // Create the "Back to Main Menu" button
  QPushButton* backButton = new QPushButton("Back", this);
  backButton->setFixedSize(200, 50);
  backButton->move(400, 475);
  backButton->setStyleSheet(buttonStyles);

  // Create label to show account status
  statusLabel = new QLabel(this);
  statusLabel->setAlignment(Qt::AlignCenter);
  statusLabel->setFixedSize(250, 50);
  statusLabel->move(375, 250);

  // Connect the button click signal to a slot
  connect(createAccountButton, &QPushButton::clicked, this,
          &CreateAccountWindow::onCreateAccountClicked);

  // // Connect the back button to navigate to the main menu
  connect(backButton, &QPushButton::clicked, this,
          &CreateAccountWindow::goBack);
}

void CreateAccountWindow::show() {
  QWidget::show();
  qDebug() << "Create account window shown";
}

void CreateAccountWindow::setPreviousScreen(QWidget* previous) {
  previousScreen = previous;
}

void CreateAccountWindow::goBack() {
  this->hide();

  if (previousScreen) {
    previousScreen->show();
  }

  emit back();
}

void CreateAccountWindow::onCreateAccountClicked() {
  // Placeholder logic for creating account, update the status label
  QString username = usernameEdit->text().trimmed();
  if (username.isEmpty()) {
    statusLabel->setText("Please enter a username.");
  } else {
    // For now, just show that the account is created
    statusLabel->setText("Account Created.");
    saveJsonFile(username);
    emit accountCreated();
  }
}

// Save JSON data
void CreateAccountWindow::saveJsonFile(const QString& username) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  QJsonObject jsonObject;

  // Read existing JSON data (if any)
  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Failed to open" << absolutePath << " for reading.";
      statusLabel->setText("Error: Could not read profile.json");
      return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isObject()) {
      jsonObject = doc.object();  // Load existing data
    }
  }

  // Check if user exists and preserve statistics
  QJsonObject userObject;
  QJsonObject statistics;

  if (jsonObject.contains(username)) {
    statusLabel->setText("Account " + username + " already exists");
    userObject = jsonObject[username].toObject();
    if (userObject.contains("statistics")) {
      statistics =
          userObject["statistics"].toObject();  // Preserve existing statistics
    }
  }

  // Initialize statistics if they don't exist
  if (statistics.isEmpty()) {
    statistics["games_played"] = 0;
    statistics["games_win"] = 0;
    statistics["guess_total"] = 0;
    statistics["guess_hit"] = 0;
  }

  // Update user object
  userObject["user_name"] = username;
  userObject["statistics"] = statistics;

  // Store the updated user object in the main JSON
  jsonObject[username] = userObject;

  // Write updated JSON data
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    statusLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson());
  file.close();
}
