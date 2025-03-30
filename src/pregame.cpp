#include "pregame.h"
#include <QScreen>
#include <QGuiApplication>

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

  game = new Game();
  users = User::instance();
  createAccountWindow = CreateAccountWindow::getInstance();

  // Create pregame window UI elements manually
  layout = new QVBoxLayout(this);
  teamsLayout = new QHBoxLayout();
  redTeamLayout = new QVBoxLayout();
  blueTeamLayout = new QVBoxLayout();
  buttonsLayout = new QHBoxLayout();

  // Create a label
  label = new QLabel("Welcome to PreGame Screen", this);
  layout->addWidget(label);

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
  redTeamLayout->addWidget(new QLabel("Red"));
  redTeamLayout->addWidget(new QLabel("Spy Master"));
  redTeamSpyMasterComboBox = new QComboBox();
  redTeamLayout->addWidget(redTeamSpyMasterComboBox);
  redTeamLayout->addWidget(new QLabel("Operative"));
  redTeamOperativeComboBox = new QComboBox();
  redTeamLayout->addWidget(redTeamOperativeComboBox);

  blueTeamLayout->addWidget(new QLabel("Blue"));
  blueTeamLayout->addWidget(new QLabel("Spy Master"));
  blueTeamSpyMasterComboBox = new QComboBox();
  blueTeamLayout->addWidget(blueTeamSpyMasterComboBox);
  blueTeamLayout->addWidget(new QLabel("Operative"));
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
}

PreGame::~PreGame() {
  // Let Qt handle widget deletion
  // changed so that qt is no longer responsible for deleting the widgets.
  delete gameBoard;
  delete game;
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

  game->addPlayer(Player(redSpyMaster, ROLE::SPYMASTER, TEAM::RED));
  game->addPlayer(Player(redOperative, ROLE::OPERATIVE, TEAM::RED));
  game->addPlayer(Player(blueSpyMaster, ROLE::SPYMASTER, TEAM::BLUE));
  game->addPlayer(Player(blueOperative, ROLE::OPERATIVE, TEAM::BLUE));

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