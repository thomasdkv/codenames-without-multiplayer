#include "pregame.h"
#include <QScreen>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

PreGame::PreGame(QWidget* parent) : QWidget(parent), gameBoard(nullptr) {
  this->setFixedSize(1000, 800);

  // Center the window on the screen
  QScreen *screen = QGuiApplication::primaryScreen();
  if (screen) {
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
  }

  users = User::instance();
  createAccountWindow = CreateAccountWindow::getInstance();

  // Create pregame window UI elements manually
  layout = new QVBoxLayout(this);
  teamsLayout = new QHBoxLayout();
  redTeamLayout = new QVBoxLayout();
  blueTeamLayout = new QVBoxLayout();
  buttonsLayout = new QHBoxLayout();

  // Create a button to go back to the main window
  backButton = new QPushButton("Back", this);
  buttonsLayout->addWidget(backButton);

  createAccountButton = new QPushButton("Create Account", this);
  buttonsLayout->addWidget(createAccountButton);

  // Create a button to start game
  startButton = new QPushButton("Start", this);
  buttonsLayout->addWidget(startButton);

  // Connect back button to a slot
  connect(backButton, &QPushButton::clicked, this, &PreGame::goBackToMain);

  connect(createAccountButton, &QPushButton::clicked, this,
          &PreGame::openCreateAccount);

  // Connect start button to a slot
  connect(startButton, &QPushButton::clicked, this, &PreGame::startGame);

  connect(createAccountWindow, &CreateAccountWindow::accountCreated, this,
          &PreGame::populateUserDropdowns);

  // Add labels for the teams
  QLabel* redLabel = new QLabel("RED", this);
  redLabel->setStyleSheet("font-weight: bold; color: red; font-size: 24px;");
  redLabel->setAlignment(Qt::AlignCenter);
  redTeamLayout->addWidget(redLabel);

  redTeamLayout->addWidget(new QLabel("Spy Master: ", this));
  redTeamSpyMasterComboBox = new QComboBox();
  redTeamLayout->addWidget(redTeamSpyMasterComboBox);
  redTeamLayout->addWidget(new QLabel("Operative: ", this));
  redTeamOperativeComboBox = new QComboBox();
  redTeamLayout->addWidget(redTeamOperativeComboBox);

  QLabel* blueLabel = new QLabel("BLUE", this);
  blueLabel->setStyleSheet("font-weight: bold; color: blue; font-size: 24px;");
  blueLabel->setAlignment(Qt::AlignCenter);
  blueTeamLayout->addWidget(blueLabel);

  blueTeamLayout->addWidget(new QLabel("Spy Master: ", this));
  blueTeamSpyMasterComboBox = new QComboBox();
  blueTeamLayout->addWidget(blueTeamSpyMasterComboBox);
  blueTeamLayout->addWidget(new QLabel("Operative: ", this));
  blueTeamOperativeComboBox = new QComboBox();
  blueTeamLayout->addWidget(blueTeamOperativeComboBox);

  redTeamLayout->setAlignment(Qt::AlignLeft);
  blueTeamLayout->setAlignment(Qt::AlignRight);

  // Align items within the layouts
  teamsLayout->addLayout(redTeamLayout);
  teamsLayout->addLayout(blueTeamLayout);

  // Align redTeamLayout to the left
  teamsLayout->setAlignment(redTeamLayout, Qt::AlignLeft);

  // Align blueTeamLayout to the right
  teamsLayout->setAlignment(blueTeamLayout, Qt::AlignRight);

  // Set the alignment of teamsLayout in the main layout (center it)
  layout->addLayout(teamsLayout);
  layout->setAlignment(teamsLayout, Qt::AlignCenter);

  // Set the alignment of buttonsLayout in the main layout (center it)
  layout->addLayout(buttonsLayout);
  layout->setAlignment(buttonsLayout, Qt::AlignCenter);

  // Set the alignment of the buttons within the buttonsLayout to the center
  buttonsLayout->setAlignment(Qt::AlignCenter);

  // Set the layout of the window
  setLayout(layout);

  gameBoard = new GameBoard("RedSpy", "RedOp", "BlueSpy", "BlueOp", nullptr);
  connect(gameBoard, &GameBoard::gameEnded, this, &PreGame::handleGameEnd);
  gameBoard->hide();

  populateUserDropdowns();

  // Button Styling
  QString buttonStyles =
      "QPushButton {"
      "   background-color: rgb(65, 42, 213);"
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

  startButton->setStyleSheet(buttonStyles);
  backButton->setStyleSheet(buttonStyles);
  createAccountButton->setStyleSheet(buttonStyles);
}

PreGame::~PreGame() {
  // Let Qt handle widget deletion
  // changed so that qt is no longer responsible for deleting the widgets.
  delete gameBoard;
}

void PreGame::populateUserDropdowns() {
  QJsonObject json = users->loadJsonFile();
  usernames = json.keys();

  redTeamSpyMasterComboBox->clear();
  redTeamSpyMasterComboBox->addItems(usernames);

  redTeamOperativeComboBox->clear();
  redTeamOperativeComboBox->addItems(usernames);

  blueTeamSpyMasterComboBox->clear();
  blueTeamSpyMasterComboBox->addItems(usernames);

  blueTeamOperativeComboBox->clear();
  blueTeamOperativeComboBox->addItems(usernames);
}

void PreGame::goBackToMain() {
  this->hide();
  emit backToMainWindow();  // Emit signal to notify MainWindow to show itself
}

void PreGame::openCreateAccount() {
  this->hide();
  createAccountWindow->setPreviousScreen(this);
  createAccountWindow->show();
}

void PreGame::startGame() {
  QString redSpyMaster = redTeamSpyMasterComboBox->currentText();
  QString redOperative = redTeamOperativeComboBox->currentText();
  QString blueSpyMaster = blueTeamSpyMasterComboBox->currentText();
  QString blueOperative = blueTeamOperativeComboBox->currentText();

  QStringList selectedUsernames = {
    redSpyMaster,
    redOperative,
    blueSpyMaster,
    blueOperative
  };

  QSet<QString> uniqueUsernames(selectedUsernames.begin(), selectedUsernames.end());
  if (uniqueUsernames.size() < selectedUsernames.size()) {
    QMessageBox::critical(this, "Error", "Duplicate usernames are not allowed!");
    return;
  }

  gameBoard->setRedSpyMasterName(redSpyMaster);
  gameBoard->setRedOperativeName(redOperative);
  gameBoard->setBlueSpyMasterName(blueSpyMaster);
  gameBoard->setBlueOperativeName(blueOperative);

  gameBoard->updateTeamLabels();
  this->hide();
  emit start();
  gameBoard->show();
}

void PreGame::show() {
  qDebug() << "Returning to PreGame screen";
  populateUserDropdowns();
  QWidget::show();
  qDebug() << "Pregame shown";
}

void PreGame::handleGameEnd() { show(); }

// Getter functions to return the text from the textboxes
QString PreGame::getRedTeamSpyMasterNickname() const {
  return redTeamSpyMasterComboBox->currentText().trimmed();
}

QString PreGame::getRedTeamOperativeNickname() const {
  return redTeamOperativeComboBox->currentText().trimmed();
}

QString PreGame::getBlueTeamSpyMasterNickname() const {
  return blueTeamSpyMasterComboBox->currentText().trimmed();
}

QString PreGame::getBlueTeamOperativeNickname() const {
  return blueTeamOperativeComboBox->currentText().trimmed();
}