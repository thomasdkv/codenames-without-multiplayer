#include "mainwindow.h"
#include "pregame.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Create central widget and layout
  centralWidget = new QWidget(this);
  layout = new QVBoxLayout(centralWidget);

  // Create a button to open the PreGame window
  pushButton = new QPushButton("Open PreGame Window", centralWidget);
  layout->addWidget(pushButton);

  // Create the PreGame window
  preGameWindow = new PreGame();

  // Connect button to open PreGame window
  connect(pushButton, &QPushButton::clicked, this, &MainWindow::openPreGame);

  // Connect signal from PreGame to show MainWindow when back button is clicked
  connect(preGameWindow, &PreGame::backToMainWindow, this,
          &MainWindow::showMainWindow);

  // Set central widget and layout
  centralWidget->setLayout(layout);
  setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() { delete preGameWindow; }

void MainWindow::openPreGame() {
  this->hide();
  preGameWindow->show();
}

void MainWindow::showMainWindow() { this->show(); }
