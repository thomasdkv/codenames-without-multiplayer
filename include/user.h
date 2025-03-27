#ifndef USER_H
#define USER_H

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

class User : public QWidget {
  Q_OBJECT

 public:
  // Singleton access method
  static User* instance(QWidget* parent = nullptr);

  ~User();

  void signUp(const QString& username);

  void updateGamesPlayed(const QString& username,
                         const unsigned int& newGamesPlayed);
  unsigned int getGamesPlayed(const QString& username) const;

  void updateWins(const QString& username, const unsigned int& newWins);
  unsigned int getWins(const QString& username) const;

  float getWinRate(const QString& username) const;

  void updateGuessTotal(const QString& username,
                        const unsigned int& newGuessTotal);
  unsigned int getGuessTotal(const QString& username) const;

  void updateGuessHit(const QString& username, const unsigned int& newGuessHit);
  unsigned int getGuessHit(const QString& username) const;

  float getHitRate(const QString& username);

  void renameUser(const QString& oldUsername, const QString& newUsername);

 private slots:
  void handleLogin();   // Handles user login
  void handleSignUp();  // Handles user registration

 private:
  explicit User(QWidget* parent = nullptr);

  QString jsonFilePath = "resources/profile.json";

  QLabel* jsonContentLabel;  // Label to display JSON content
  QLineEdit* usernameInput;
  // QLineEdit* passwordInput;
  QPushButton* loginButton;
  QPushButton* signUpButton;

  const QString SALT = "Ashton Hall";

  void loadJsonFile();  // Function to load JSON data
  void saveJsonFile(const QString& username);
  // QString hashPassword(const QString& password);
  // bool verifyPassword(const QString& inputPassword, const QString&
  // storedHash);
};

#endif  // USER_H
