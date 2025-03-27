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

  // passwordInput = new QLineEdit(this);
  // passwordInput->setPlaceholderText("Enter Password");
  // passwordInput->setEchoMode(QLineEdit::Password);

  loginButton = new QPushButton("Login", this);
  signUpButton = new QPushButton("Sign Up", this);

  layout->addWidget(usernameInput);
  // layout->addWidget(passwordInput);
  layout->addWidget(loginButton);
  layout->addWidget(signUpButton);

  setLayout(layout);

  connect(loginButton, &QPushButton::clicked, this, &User::handleLogin);
  connect(signUpButton, &QPushButton::clicked, this, &User::handleSignUp);

  QString name = "thomas";
  signUp(name);
  updateGamesPlayed(name, 100);
  updateWins(name, 99);
  updateGuessTotal(name, 20);
  updateGuessHit(name, 10);

  loadJsonFile();
}

User::~User() {}

void User::loadJsonFile() {
  QFile file(jsonFilePath);
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
  if (!jsonObject.contains("user_name")) {
    jsonContentLabel->setText("Profile incomplete. Please sign up.");
    return;
  }

  jsonContentLabel->setText("Profile found. Please log in.");
}

// Hashing function with salt
// QString User::hashPassword(const QString& password) {
//   QByteArray saltedPassword = (password + SALT).toUtf8();
//   QByteArray hashed =
//       QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256);
//   return hashed.toHex();
// }

// Verify hashed password
// bool User::verifyPassword(const QString& inputPassword,
//                           const QString& storedHash) {
//   return hashPassword(inputPassword) == storedHash;
// }

// Save JSON data
void User::saveJsonFile(const QString& username) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  QJsonObject jsonObject;

  // Read existing JSON data (if any)
  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Failed to open" << absolutePath << " for reading.";
      jsonContentLabel->setText("Error: Could not read profile.json");
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
    jsonContentLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson());
  file.close();
}

// Handle user sign-up
void User::handleSignUp() {
  QString username = usernameInput->text().trimmed();
  // QString password = passwordInput->text().trimmed();

  if (username.isEmpty()) {
    jsonContentLabel->setText("Username and password cannot be empty.");
    return;
  }

  // QString hashedPassword = hashPassword(password);
  saveJsonFile(username);

  jsonContentLabel->setText("Account created. Please log in.");
}

void User::signUp(const QString& username) { saveJsonFile(username); }

void User::handleLogin() {
  QFile file(jsonFilePath);  // Use the relative path

  if (!file.open(QIODevice::ReadOnly)) {
    jsonContentLabel->setText("Error: Could not open profile.json");
    qDebug() << "Failed to open profile.json";
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  qDebug() << "Raw JSON data:"
           << jsonData;  // Print the raw content for debugging

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    jsonContentLabel->setText("Error: Invalid JSON format");
    qDebug() << "Invalid JSON format. Document is not an object.";
    return;
  }

  QJsonObject jsonObject = doc.object();
  QString enteredUsername = usernameInput->text().trimmed();
  // QString enteredPassword = passwordInput->text().trimmed();

  if (!jsonObject.contains(enteredUsername)) {
    jsonContentLabel->setText("Login failed. User not found.");
    qDebug() << "User not found: " << enteredUsername;
    return;
  }

  QJsonObject userObject = jsonObject[enteredUsername].toObject();
  QString storedUsername = userObject["user_name"].toString();
  // QString storedHashedPassword = userObject["hashed-password"].toString();

  qDebug() << "Stored Username: " << storedUsername;
  // qDebug() << "Stored Hashed Password: " << storedHashedPassword;

  if (enteredUsername == storedUsername) {
    jsonContentLabel->setText("Login successful. Welcome, " + storedUsername +
                              "!");
    qDebug() << "User logged in: " << storedUsername;
  } else {
    jsonContentLabel->setText("Login failed. Check credentials.");
    qDebug() << "Login failed for user: " << enteredUsername;
  }
}

int User::getGamesPlayed(const QString& username) const{
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return -1;  // Indicates error
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return -1;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return -1;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return -1;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Check if "statistics" exists and contains "games_win"
  if (!userObject.contains("statistics") ||
      !userObject["statistics"].isObject()) {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return -1;
  }

  QJsonObject statisticsObject = userObject["statistics"].toObject();
  int gamesPlayed = statisticsObject.contains("games_played")
                 ? statisticsObject["games_played"].toInt()
                 : 0;

  qDebug() << "Retrieved games played for user:" << username << "| Games played count:" << gamesPlayed;
  return gamesPlayed;
}

void User::updateGamesPlayed(const QString& username, const int& newGamesPlayed) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return;
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Ensure statistics object exists
  QJsonObject statisticsObject;
  if (userObject.contains("statistics") &&
      userObject["statistics"].isObject()) {
    statisticsObject = userObject["statistics"].toObject();
  } else {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return;
  }

  // Update wins count
  statisticsObject["games_played"] = newGamesPlayed;
  userObject["statistics"] = statisticsObject;
  jsonObject[username] = userObject;

  // Write updated JSON data
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    jsonContentLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson(QJsonDocument::Indented));
  file.close();

  qDebug() << "Updated games played for user:" << username
           << "| New games played count:" << statisticsObject["games_played"].toInt();
  jsonContentLabel->setText("Games played count updated for " + username);
}


int User::getWins(const QString& username) const{
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return -1;  // Indicates error
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return -1;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return -1;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return -1;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Check if "statistics" exists and contains "games_win"
  if (!userObject.contains("statistics") ||
      !userObject["statistics"].isObject()) {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return -1;
  }

  QJsonObject statisticsObject = userObject["statistics"].toObject();
  int wins = statisticsObject.contains("games_win")
                 ? statisticsObject["games_win"].toInt()
                 : 0;

  qDebug() << "Retrieved wins for user:" << username << "| Wins count:" << wins;
  return wins;
}

void User::updateWins(const QString& username, const int& newWins) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return;
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Ensure statistics object exists
  QJsonObject statisticsObject;
  if (userObject.contains("statistics") &&
      userObject["statistics"].isObject()) {
    statisticsObject = userObject["statistics"].toObject();
  } else {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return;
  }

  // Update wins count
  statisticsObject["games_win"] = newWins;
  userObject["statistics"] = statisticsObject;
  jsonObject[username] = userObject;

  // Write updated JSON data
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    jsonContentLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson(QJsonDocument::Indented));
  file.close();

  qDebug() << "Updated wins for user:" << username
           << "| New wins count:" << statisticsObject["games_win"].toInt();
  jsonContentLabel->setText("Win count updated for " + username);
}

int User::getGuessTotal(const QString& username) const{
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return -1;  // Indicates error
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return -1;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return -1;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return -1;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Check if "statistics" exists and contains "games_win"
  if (!userObject.contains("statistics") ||
      !userObject["statistics"].isObject()) {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return -1;
  }

  QJsonObject statisticsObject = userObject["statistics"].toObject();
  int guessTotal = statisticsObject.contains("guess_total")
                 ? statisticsObject["guess_total"].toInt()
                 : 0;

  qDebug() << "Retrieved total number of guess for user:" << username << "| Guess total count:" << guessTotal;
  return guessTotal;
}

void User::updateGuessTotal(const QString& username, const int& newGuessTotal) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return;
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Ensure statistics object exists
  QJsonObject statisticsObject;
  if (userObject.contains("statistics") &&
      userObject["statistics"].isObject()) {
    statisticsObject = userObject["statistics"].toObject();
  } else {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return;
  }

  // Update wins count
  statisticsObject["guess_total"] = newGuessTotal;
  userObject["statistics"] = statisticsObject;
  jsonObject[username] = userObject;

  // Write updated JSON data
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    jsonContentLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson(QJsonDocument::Indented));
  file.close();

  qDebug() << "Updated guess total for user:" << username
           << "| New guess total count:" << statisticsObject["guess_total"].toInt();
  jsonContentLabel->setText("Guess total count updated for " + username);
}

int User::getGuessHit(const QString& username) const{
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return -1;  // Indicates error
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return -1;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return -1;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return -1;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Check if "statistics" exists and contains "games_win"
  if (!userObject.contains("statistics") ||
      !userObject["statistics"].isObject()) {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return -1;
  }

  QJsonObject statisticsObject = userObject["statistics"].toObject();
  int guessHit = statisticsObject.contains("guess_hit")
                 ? statisticsObject["guess_hit"].toInt()
                 : 0;

  qDebug() << "Retrieved guess hit for user:" << username << "| Guess hit count:" << guessHit;
  return guessHit;
}

void User::updateGuessHit(const QString& username, const int& newGuessHit) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return;
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return;
  }

  QJsonObject jsonObject = doc.object();

  // Check if user exists
  if (!jsonObject.contains(username)) {
    qDebug() << "User not found:" << username;
    jsonContentLabel->setText("Error: User does not exist.");
    return;
  }

  // Get user object
  QJsonObject userObject = jsonObject[username].toObject();

  // Ensure statistics object exists
  QJsonObject statisticsObject;
  if (userObject.contains("statistics") &&
      userObject["statistics"].isObject()) {
    statisticsObject = userObject["statistics"].toObject();
  } else {
    qDebug() << "Error: No statistics found for user:" << username;
    jsonContentLabel->setText("Error: User statistics missing.");
    return;
  }

  // Update wins count
  statisticsObject["guess_hit"] = newGuessHit;
  userObject["statistics"] = statisticsObject;
  jsonObject[username] = userObject;

  // Write updated JSON data
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    jsonContentLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson(QJsonDocument::Indented));
  file.close();

  qDebug() << "Updated guess hit for user:" << username
           << "| New guess hit count:" << statisticsObject["guess_hit"].toInt();
  jsonContentLabel->setText("Guess hit count updated for " + username);
}


void User::renameUser(const QString& oldUsername, const QString& newUsername) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  if (!file.exists()) {
    qDebug() << "Error: profile.json does not exist.";
    jsonContentLabel->setText("Error: No user data found.");
    return;
  }

  // Read existing JSON data
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open" << absolutePath << " for reading.";
    jsonContentLabel->setText("Error: Could not read profile.json");
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    qDebug() << "Invalid JSON format.";
    jsonContentLabel->setText("Error: Invalid profile format.");
    return;
  }

  QJsonObject jsonObject = doc.object();

  // Check if the old username exists
  if (!jsonObject.contains(oldUsername)) {
    qDebug() << "User not found:" << oldUsername;
    jsonContentLabel->setText("Error: User does not exist.");
    return;
  }

  // Check if the new username already exists
  if (jsonObject.contains(newUsername)) {
    qDebug() << "New username already exists:" << newUsername;
    jsonContentLabel->setText("Error: Username already taken.");
    return;
  }

  // Rename the user: Move data from old username to new username
  QJsonObject userObject = jsonObject[oldUsername].toObject();

  // Ensure profile object exists
  if (userObject.contains("profile") && userObject["profile"].isObject()) {
    QJsonObject profileObject = userObject["profile"].toObject();
    profileObject["player_name"] = newUsername;  // Update stored player_name
    userObject["profile"] = profileObject;
  } else {
    qDebug() << "Error: No profile data found for user:" << oldUsername;
    jsonContentLabel->setText("Error: User profile missing.");
    return;
  }

  jsonObject.remove(oldUsername);        // Remove old entry
  jsonObject[newUsername] = userObject;  // Insert under new username

  // Write updated JSON data
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    jsonContentLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson(QJsonDocument::Indented));
  file.close();

  qDebug() << "User renamed from" << oldUsername << "to" << newUsername;
  jsonContentLabel->setText("Username successfully changed.");
}
