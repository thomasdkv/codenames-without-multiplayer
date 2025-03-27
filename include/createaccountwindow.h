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

class CreateAccountWindow : public QWidget {
  Q_OBJECT

 public:
  static CreateAccountWindow* getInstance(QWidget* parent = nullptr);

 private:
  explicit CreateAccountWindow(QWidget* parent = nullptr);
  static CreateAccountWindow* instance;

 private slots:
  void onCreateAccountClicked();
  void goBack();

 signals:
  void back();

 private:
  void saveJsonFile(const QString& username);

  QLineEdit* usernameEdit;
  QPushButton* createAccountButton;
  QLabel* statusLabel;

  QString jsonFilePath = "resources/profile.json";  // Update path as necessary
};

#endif  // CREATEACCOUNTWINDOW_H
