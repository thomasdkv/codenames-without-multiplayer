#include "mainwindow.h"
#include "pregame.h"
#include "Multiplayer/multimain.h"
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QPalette>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  // Create central widget and layout
  centralWidget = new QWidget(this);

  this->setStyleSheet(
      "background-image: url(:/images/menu-background.png);"
      "background-position: center;");

  // Ensure centralWidget has a solid background
  centralWidget->setStyleSheet(
      "QWidget {"
      "   background: transparent;"  // Make the central widget transparent
      "}");

  // Title label
  titleLabel = new QLabel("C++Names", centralWidget);
  titleLabel->setFixedSize(600, 50);
  titleLabel->move(375, 50);

  // Button to open local play pre game lobby
  localPlayButton = new QPushButton("Local Play", centralWidget);
  localPlayButton->setFixedSize(200, 50);
  localPlayButton->move(400, 200);  // Set y-position to 50
  QGraphicsDropShadowEffect* shadowEffectLocal = new QGraphicsDropShadowEffect;
  shadowEffectLocal->setBlurRadius(5);     // Blur radius
  shadowEffectLocal->setOffset(0, 3);      // Shadow offset
  shadowEffectLocal->setColor(Qt::black);  // Shadow color
  localPlayButton->setGraphicsEffect(shadowEffectLocal);

  // Button to open online play pre game lobby
  onlinePlayButton = new QPushButton("Online Play", centralWidget);
  onlinePlayButton->setFixedSize(200, 50);
  onlinePlayButton->move(400, 275);  // Set y-position to 50
  QGraphicsDropShadowEffect* shadowEffectOnline = new QGraphicsDropShadowEffect;
  shadowEffectOnline->setBlurRadius(5);     // Blur radius
  shadowEffectOnline->setOffset(0, 3);      // Shadow offset
  shadowEffectOnline->setColor(Qt::black);  // Shadow color
  onlinePlayButton->setGraphicsEffect(shadowEffectOnline);

  // Button to open tutorial
  tutorialButton = new QPushButton("Tutorial", centralWidget);
  tutorialButton->setFixedSize(200, 50);
  tutorialButton->move(400, 425);  // Position below other buttons
  QGraphicsDropShadowEffect* shadowEffectTutorial = new QGraphicsDropShadowEffect;
  shadowEffectTutorial->setBlurRadius(5);     // Blur radius
  shadowEffectTutorial->setOffset(0, 3);      // Shadow offset
  shadowEffectTutorial->setColor(Qt::black);  // Shadow color
  tutorialButton->setGraphicsEffect(shadowEffectTutorial);

  // Button to open statistics menu
  statsButton = new QPushButton("Statistics", centralWidget);
  statsButton->setFixedSize(200, 50);
  statsButton->move(400, 350);  // Set y-position to 50
  QGraphicsDropShadowEffect* shadowEffectStats = new QGraphicsDropShadowEffect;
  shadowEffectStats->setBlurRadius(5);     // Blur radius
  shadowEffectStats->setOffset(0, 3);      // Shadow offset
  shadowEffectStats->setColor(Qt::black);  // Shadow color
  statsButton->setGraphicsEffect(shadowEffectStats);

  // Button to open Create Account menu
  createAccountButton = new QPushButton("Create Account", centralWidget);
  createAccountButton->setFixedSize(200, 50);
  createAccountButton->move(400, 500);  // Set y-position to 50
  QGraphicsDropShadowEffect* shadowEffectAccount =
      new QGraphicsDropShadowEffect;
  shadowEffectAccount->setBlurRadius(5);     // Blur radius
  shadowEffectAccount->setOffset(0, 3);      // Shadow offset
  shadowEffectAccount->setColor(Qt::black);  // Shadow color
  createAccountButton->setGraphicsEffect(shadowEffectAccount);

  // Styling
  titleLabel->setStyleSheet("font-weight: bold; font-size: 50px;");

  // Button Styling
  QString buttonStyles =
      "QPushButton {"
      "   background-color:rgb(65, 42, 213);"
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

  localPlayButton->setStyleSheet(buttonStyles);
  onlinePlayButton->setStyleSheet(buttonStyles);
  tutorialButton->setStyleSheet(buttonStyles);
  statsButton->setStyleSheet(buttonStyles);
  createAccountButton->setStyleSheet(buttonStyles);

  // Create the PreGame window
  preGameWindow = new PreGame();
  // Connect button to open PreGame window
  connect(localPlayButton, &QPushButton::clicked, this,
          &MainWindow::openPreGame);

  // Connect signal from PreGame to show MainWindow when back button is clicked
  connect(preGameWindow, &PreGame::backToMainWindow, this,
          &MainWindow::showMainWindow);


   multiMain = new MultiMain();
  // Connect button to open Multiplayer window
  connect(onlinePlayButton, &QPushButton::clicked, this,
          &MainWindow::openOnlineGame);

  statsWindow = new StatisticsWindow();
  connect(statsButton, &QPushButton::clicked, this,
          &MainWindow::openStatsWindow);
  connect(statsWindow, &StatisticsWindow::backToMainWindow, this,
          &MainWindow::showMainWindow);

  createAccountWindow = CreateAccountWindow::getInstance();
  connect(createAccountButton, &QPushButton::clicked, this,
          &MainWindow::openCreateAccount);

  //   disconnect(createAccountWindow, &CreateAccountWindow::back, nullptr,
  //   nullptr); connect(createAccountWindow, &CreateAccountWindow::back, this,
  //           &MainWindow::showMainWindow);

  onlineGameWindow = User::instance();
  connect(onlineGameWindow, &User::backToMainMenu, this,
          &MainWindow::showMainWindow);

  // Create the Tutorial window
  tutorialWindow = new Tutorial();
  
  // Make sure these connections exist
  connect(tutorialButton, &QPushButton::clicked, this, &MainWindow::openTutorial);
  connect(tutorialWindow, &Tutorial::tutorialClosed, this, &MainWindow::showMainWindow);

  // Set central widget and layout
  setCentralWidget(centralWidget);
}

void MainWindow::openMultiMain() {
    this->hide();
    multiMain->show();
}

MainWindow::~MainWindow() {
  delete preGameWindow;
  delete tutorialWindow;
}

void MainWindow::openPreGame() {
  this->hide();
  preGameWindow->show();
}

void MainWindow::showMainWindow() {
  qDebug() << "ShowMainWindow called";
  this->show();
}

void MainWindow::openOnlineGame() {
  // Show the login/signup screen
  this->hide();
  multiMain->show();
}

void MainWindow::openStatsWindow() {
  this->hide();
  statsWindow->show();
}

void MainWindow::openCreateAccount() {
  this->hide();
  createAccountWindow->setPreviousScreen(this);
  createAccountWindow->show();
}

void MainWindow::openTutorial() {
  this->hide();
  tutorialWindow->show();
}