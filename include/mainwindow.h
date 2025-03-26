#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMainWindow>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

class PreGame;  // Forward declaration of PreGame class
class User;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void openPreGame();
  void showMainWindow();
  void openOnlineGame();

 private:
  QWidget* centralWidget;
  QVBoxLayout* layout;

  QLabel* titleLabel;

  QPushButton* localPlayButton;
  QPushButton* onlinePlayButton;
  QPushButton* statsButton;

  PreGame* preGameWindow;  // PreGame pointer to the second window
  User* onlineGameWindow;
};

#endif  // MAINWINDOW_H