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

  void updateWins(const QString& username, const int& newWins);
  int getWins(const QString& username);

  void renameUser(const QString& oldUsername, const QString& newUsername);

 private slots:
  void handleLogin();   // Handles user login
  void handleSignUp();  // Handles user registration

 private:
  explicit User(QWidget* parent = nullptr);

  QLabel* jsonContentLabel;  // Label to display JSON content
  QLineEdit* usernameInput;
  // QLineEdit* passwordInput;
  QPushButton* loginButton;
  QPushButton* signUpButton;

  const QString SALT = "Ashton Hall";

  void loadJsonFile();  // Function to load JSON data
  void saveJsonFile(const QString& username);
  // QString hashPassword(const QString& password);
  // bool verifyPassword(const QString& inputPassword, const QString& storedHash);
};

#endif  // USER_H
