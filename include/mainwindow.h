#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMainWindow>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

#include "createaccountwindow.h"
#include "pregame.h"
#include "user.h"
#include "statisticswindow.h"

class PreGame;
class User;
class CreateAccountWindow;
class StatisticsWindow;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 public slots:
  void showMainWindow();

 private slots:
  void openPreGame();
  void openOnlineGame();
  void openStatsWindow();
  void openCreateAccount();

 private:
  QWidget* centralWidget;
  QVBoxLayout* layout;

  QLabel* titleLabel;

  QPushButton* localPlayButton;
  QPushButton* onlinePlayButton;
  QPushButton* statsButton;
  QPushButton* createAccountButton;

  PreGame* preGameWindow;  // PreGame pointer to the second window
  User* onlineGameWindow;
  CreateAccountWindow* createAccountWindow;
  StatisticsWindow* statsWindow;
};

#endif  // MAINWINDOW_H