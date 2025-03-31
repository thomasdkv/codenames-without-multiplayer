/**
 * @file mainwindow.h
 * @brief Declaration of the MainWindow class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>

#include "Multiplayer/multimain.h"
#include "createaccountwindow.h"
#include "pregame.h"
#include "statisticswindow.h"
#include "tutorial.h"
#include "user.h"

class PreGame;
class User;
class CreateAccountWindow;
class StatisticsWindow;
class Tutorial;
class MultiMain;

/**
 * @class MainWindow
 * @brief The main application window.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for MainWindow.
   * @param parent The parent widget (default is nullptr).
   */
  explicit MainWindow(QWidget* parent = nullptr);

  /**
   * @brief Destructor for MainWindow.
   */
  ~MainWindow();

 public slots:
  /**
   * @brief Displays the main window.
   */
  void showMainWindow();

 private slots:
  /**
   * @brief Opens the PreGame window.
   */
  void openPreGame();

  /**
   * @brief Opens the online game window.
   */
  void openOnlineGame();

  /**
   * @brief Opens the statistics window.
   */
  void openStatsWindow();

  /**
   * @brief Opens the Create Account window.
   */
  void openCreateAccount();

  /**
   * @brief Opens the Tutorial window.
   */
  void openTutorial();

  /**
   * @brief Opens the Multiplayer main window.
   */
  void openMultiMain();

 private:
  QWidget* centralWidget;  ///< Pointer to the central widget, which holds all
                           ///< main UI elements.
  QVBoxLayout* layout;     ///< Layout for organizing the widgets vertically.

  QLabel* titleLabel;  ///< Label displaying the application title.

  PreGame* preGameWindow;  ///< Pointer to the PreGame window for local gameplay
                           ///< setup.
  MultiMain* multiMain;    ///< Pointer to the Multiplayer main window.
  QPushButton* localPlayButton;   ///< Button for starting a local game.
  QPushButton* onlinePlayButton;  ///< Button for starting an online game.
  QPushButton* tutorialButton;    ///< Button for opening the tutorial.
  QPushButton* statsButton;       ///< Button for opening the statistics window.
  QPushButton*
      createAccountButton;  ///< Button for opening the account creation window.

  User* onlineGameWindow;  ///< Pointer to the online game window where users
                           ///< can play multiplayer.
  CreateAccountWindow*
      createAccountWindow;  ///< Pointer to the account creation window.
  StatisticsWindow*
      statsWindow;  ///< Pointer to the statistics window displaying game stats.
  Tutorial* tutorialWindow;  ///< Pointer to the tutorial window explaining the
                             ///< game mechanics.
};

#endif  // MAINWINDOW_H
