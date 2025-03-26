#include "pregame.h"

PreGame::PreGame(QWidget* parent) : QWidget(parent) {
  this->setFixedSize(1000, 600);

  game = new Game();

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
  backButton = new QPushButton("Back to Main Window", this);
  buttonsLayout->addWidget(backButton);

  // Create a button to start game
  startButton = new QPushButton("Start", this);
  buttonsLayout->addWidget(startButton);

  // Connect back button to a slot
  connect(backButton, &QPushButton::clicked, this, &PreGame::goBackToMain);

  // Connect start button to a slot
  connect(startButton, &QPushButton::clicked, this, &PreGame::startGame);

  // Add labels for the teams
  redTeamLayout->addWidget(new QLabel("Red"));
  redTeamLayout->addWidget(new QLabel("Spy Master"));
  redTeamSpyMasterNickname = new QLineEdit();
  redTeamLayout->addWidget(redTeamSpyMasterNickname);
  redTeamLayout->addWidget(new QLabel("Operative"));
  redTeamOperativeNickname = new QLineEdit();
  redTeamLayout->addWidget(redTeamOperativeNickname);

  blueTeamLayout->addWidget(new QLabel("Blue"));
  blueTeamLayout->addWidget(new QLabel("Spy Master"));
  blueTeamSpyMasterNickname = new QLineEdit();
  blueTeamLayout->addWidget(blueTeamSpyMasterNickname);
  blueTeamLayout->addWidget(new QLabel("Operative"));
  blueTeamOperativeNickname = new QLineEdit();
  blueTeamLayout->addWidget(blueTeamOperativeNickname);

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
}

PreGame::~PreGame() {
  // Let Qt handle widget deletion
}

void PreGame::goBackToMain() {
  this->hide();
  emit backToMainWindow();  // Emit signal to notify MainWindow to show itself
}

void PreGame::startGame() {
  // Get the text from the textboxes
  QString redSpyMaster = getRedTeamSpyMasterNickname();
  QString redOperative = getRedTeamOperativeNickname();
  QString blueSpyMaster = getBlueTeamSpyMasterNickname();
  QString blueOperative = getBlueTeamOperativeNickname();

  game->addPlayer(Player(redSpyMaster, ROLE::SPYMASTER, TEAM::RED));
  game->addPlayer(Player(redOperative, ROLE::OPERATIVE, TEAM::RED));
  game->addPlayer(Player(blueSpyMaster, ROLE::SPYMASTER, TEAM::BLUE));
  game->addPlayer(Player(blueOperative, ROLE::OPERATIVE, TEAM::BLUE));

  this->hide();
  emit start();

  // Add the game board transition
  GameBoard* gameBoard =
      new GameBoard(redSpyMaster, redOperative, blueSpyMaster, blueOperative);
  gameBoard->show();

  qDebug() << "Connection to GameBoard successful";
}

void PreGame::showPreGame() {
  qDebug() << "Returning to PreGame screen";
  this->show();
}

// Getter functions to return the text from the textboxes
QString PreGame::getRedTeamSpyMasterNickname() const {
  return redTeamSpyMasterNickname->text();
}

QString PreGame::getRedTeamOperativeNickname() const {
  return redTeamOperativeNickname->text();
}

QString PreGame::getBlueTeamSpyMasterNickname() const {
  return blueTeamSpyMasterNickname->text();
}

QString PreGame::getBlueTeamOperativeNickname() const {
  return blueTeamOperativeNickname->text();
}