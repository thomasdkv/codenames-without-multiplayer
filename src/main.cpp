#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Initialize the Qt application

    MainWindow game;  // Create an instance of the Minesweeper game
    game.setWindowTitle("Codenames");  // Set the window title
    game.show();  // Display the game window

    return app.exec();  // Enter the Qt event loop
}