#include "localstart.h"
#include <QDebug>

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

  proceedButton = new QPushButton("Proceed", this);
  layout->addWidget(proceedButton);
  connect(proceedButton, &QPushButton::clicked, this, &LocalStart::onProceedClicked);

  backButton = new QPushButton("Back to PreGame", this);
  layout->addWidget(backButton);
  connect(backButton, &QPushButton::clicked, this, &LocalStart::onBackClicked);

  setLayout(layout);
  resize(1000, 600);
}

void LocalStart::onProceedClicked() {
  qDebug() << "Proceed to Game clicked";
  emit proceedToGame();
  close();
}

void LocalStart::onBackClicked() {
  qDebug() << "Back to PreGame clicked";
  emit backToPreGame();
  close();
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
