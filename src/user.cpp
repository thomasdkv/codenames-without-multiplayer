#include "user.h"

User* User::instance(QWidget* parent) {
  static User* _instance = nullptr;
  if (!_instance) {
    _instance = new User(parent);
  }
  return _instance;
}

User::User(QWidget* parent) : QWidget(parent) {
  this->setFixedSize(1000, 600);

  QVBoxLayout* layout = new QVBoxLayout(this);
  jsonContentLabel = new QLabel("Loading...", this);

  layout->addWidget(jsonContentLabel);

  usernameInput = new QLineEdit(this);
  usernameInput->setPlaceholderText("Enter Username");

  passwordInput = new QLineEdit(this);
  passwordInput->setPlaceholderText("Enter Password");
  passwordInput->setEchoMode(QLineEdit::Password);

  loginButton = new QPushButton("Login", this);
  signUpButton = new QPushButton("Sign Up", this);

  layout->addWidget(usernameInput);
  layout->addWidget(passwordInput);
  layout->addWidget(loginButton);
  layout->addWidget(signUpButton);

  setLayout(layout);

  connect(loginButton, &QPushButton::clicked, this, &User::handleLogin);
  connect(signUpButton, &QPushButton::clicked, this, &User::handleSignUp);

  loadJsonFile();
}

User::~User() {}

void User::loadJsonFile() {
  QFile file("resources/profile.json");
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists() || file.size() == 0) {
    jsonContentLabel->setText("No profile found. Please sign up.");
    return;
  }

  if (!file.open(QIODevice::ReadOnly)) {
    jsonContentLabel->setText("Error: Could not open profile.json");
    qDebug() << "Failed to open " << absolutePath;
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    jsonContentLabel->setText("Error: Invalid JSON format");
    qDebug() << "Invalid JSON format";
    return;
  }

  QJsonObject jsonObject = doc.object();
  if (!jsonObject.contains("user_name") ||
      !jsonObject.contains("hashed-password")) {
    jsonContentLabel->setText("Profile incomplete. Please sign up.");
    return;
  }

  jsonContentLabel->setText("Profile found. Please log in.");
}

// Hashing function with salt
QString User::hashPassword(const QString& password) {
  QByteArray saltedPassword = (password + SALT).toUtf8();
  QByteArray hashed =
      QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256);
  return hashed.toHex();
}

// Verify hashed password
bool User::verifyPassword(const QString& inputPassword,
                          const QString& storedHash) {
  return hashPassword(inputPassword) == storedHash;
}

// Save JSON data
void User::saveJsonFile(const QString& username,
                        const QString& hashedPassword) {
  QFile file("resources/profile.json");
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    jsonContentLabel->setText("Error: Could not write to profile.json");
    return;
  }
  QJsonObject jsonObject;
  jsonObject["user_name"] = username;
  jsonObject["hashed-password"] = hashedPassword;
  jsonObject["wins"] = 0;

  QJsonDocument doc(jsonObject);
  file.write(doc.toJson());
  file.close();
}

// Handle user sign-up
void User::handleSignUp() {
  QString username = usernameInput->text().trimmed();
  QString password = passwordInput->text().trimmed();

  if (username.isEmpty() || password.isEmpty()) {
    jsonContentLabel->setText("Username and password cannot be empty.");
    return;
  }

  QString hashedPassword = hashPassword(password);
  saveJsonFile(username, hashedPassword);

  jsonContentLabel->setText("Account created. Please log in.");
}

void User::handleLogin() {
  QFile file("resources/profile.json");  // Use the relative path

  if (!file.open(QIODevice::ReadOnly)) {
    jsonContentLabel->setText("Error: Could not open profile.json");
    qDebug() << "Failed to open profile.json";
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  qDebug() << "Raw JSON data:"
           << jsonData;  // Print the raw content of the file for debugging

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    jsonContentLabel->setText("Error: Invalid JSON format");
    qDebug() << "Invalid JSON format. Document is not an object.";
    return;
  }

  QJsonObject jsonObject = doc.object();
  QString storedUsername = jsonObject["user_name"].toString();
  QString storedHashedPassword = jsonObject["hashed-password"].toString();

  qDebug() << "Stored Username: " << storedUsername;
  qDebug() << "Stored Hashed Password: " << storedHashedPassword;

  QString enteredUsername = usernameInput->text().trimmed();
  QString enteredPassword = passwordInput->text().trimmed();

  if (enteredUsername == storedUsername &&
      verifyPassword(enteredPassword, storedHashedPassword)) {
    jsonContentLabel->setText("Login successful. Welcome, " + storedUsername +
                              "!");
    qDebug() << "User logged in: " << storedUsername;
  } else {
    jsonContentLabel->setText("Login failed. Check credentials.");
    qDebug() << "Login failed for user: " << enteredUsername;
  }
}
