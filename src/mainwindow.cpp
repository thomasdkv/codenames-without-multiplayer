#include "mainwindow.h"
#include "pregame.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Create central widget and layout
    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);

    // Button to open local play pre game lobby
    localPlayButton = new QPushButton("Local Play", centralWidget);
    localPlayButton->setFixedSize(200, 50);
    localPlayButton->move(400, 200); // Set y-position to 50
    
    // Button to open online play pre game lobby
    onlinePlayButton = new QPushButton("Online Play", centralWidget);
    onlinePlayButton->setFixedSize(200, 50);
    onlinePlayButton->move(400, 275); // Set y-position to 50

    // Button to open statistics menu
    statsButton = new QPushButton("Statistics", centralWidget);
    statsButton->setFixedSize(200, 50);
    statsButton->move(400, 350); // Set y-position to 50

    localPlayButton->setStyleSheet("background-color: blue;");
    onlinePlayButton->setStyleSheet("background-color: blue;");
    statsButton->setStyleSheet("background-color: blue;");

    // Create the PreGame window
    preGameWindow = new PreGame();
    
    // Connect button to open PreGame window
    connect(localPlayButton, &QPushButton::clicked, this, &MainWindow::openPreGame);
    
    // Connect signal from PreGame to show MainWindow when back button is clicked
    connect(preGameWindow, &PreGame::backToMainWindow, this, &MainWindow::showMainWindow);
    
    // Set central widget and layout
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {
    delete preGameWindow;
}

void MainWindow::openPreGame() {
    this->hide();
    preGameWindow->show();
}

void MainWindow::showMainWindow() {
    this->show();
}