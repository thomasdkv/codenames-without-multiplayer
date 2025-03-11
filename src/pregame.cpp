#include "../include/pregame.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// Make sure the Q_OBJECT macro is included in the header

PreGame::PreGame(QWidget *parent) : QWidget(parent) {
  // Create pregame window UI elements manually
  layout = new QVBoxLayout(this);

  // Create a label
  label = new QLabel("Welcome to PreGame Screen", this);
  layout->addWidget(label);

  // Create a button to go back to the main window
  backButton = new QPushButton("Back to Main Window", this);
  layout->addWidget(backButton);

  // Connect back button to a slot
  connect(backButton, &QPushButton::clicked, this, &PreGame::goBackToMain);

  setLayout(layout);
}

PreGame::~PreGame() {
  // Let Qt handle widget deletion
}

void PreGame::goBackToMain() {
  this->hide();
  emit backToMainWindow(); // Emit signal to notify MainWindow to show itself
}
