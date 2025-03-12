#include "localstart.h"

LocalStart::LocalStart(QWidget* parent) : QWidget(parent) {
  setWindowTitle("Local Start Screen");
  // Set up the layout
  QVBoxLayout* layout = new QVBoxLayout(this);

  // Initialize QLabel widgets to display the team names
  redSpyMasterLabel = new QLabel(this);
  redOperativeLabel = new QLabel(this);
  blueSpyMasterLabel = new QLabel(this);
  blueOperativeLabel = new QLabel(this);

  // Add the labels to the layout
  layout->addWidget(redSpyMasterLabel);
  layout->addWidget(redOperativeLabel);
  layout->addWidget(blueSpyMasterLabel);
  layout->addWidget(blueOperativeLabel);

  setLayout(layout);
  resize(300, 200);
}

void LocalStart::setRedTeamSpyMaster(const QString& name) {
  redSpyMasterLabel->setText("Red Spy Master: " + name);
}

void LocalStart::setRedTeamOperative(const QString& name) {
  redOperativeLabel->setText("Red Operative: " + name);
}

void LocalStart::setBlueTeamSpyMaster(const QString& name) {
  blueSpyMasterLabel->setText("Blue Spy Master: " + name);
}

void LocalStart::setBlueTeamOperative(const QString& name) {
  blueOperativeLabel->setText("Blue Operative: " + name);
}
