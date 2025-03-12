#include "mainwindow.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv); // Initialize the Qt application

  MainWindow game; // Create an instance of the application
  game.setWindowTitle("Codenames"); // Set the window title
  
  game.setFixedSize(1000, 600);

  game.show(); // Display the game window

  return app.exec(); // Enter the Qt event loop
}
