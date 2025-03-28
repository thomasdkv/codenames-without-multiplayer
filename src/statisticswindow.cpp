#include "statisticswindow.h"

StatisticsWindow::StatisticsWindow(QWidget* parent) : QWidget(parent) {
  this->setFixedSize(1000, 600);

  users = User::instance();

  QVBoxLayout* layout = new QVBoxLayout(this);

  backToMainButton = new QPushButton("Back to Main Menu");
  layout->addWidget(backToMainButton);

  QHBoxLayout* dropDownLayout = new QHBoxLayout();

  usernameComboBox = new QComboBox();
  dropDownLayout->addWidget(usernameComboBox);
  showUserStatsButton = new QPushButton();
  dropDownLayout->addWidget(showUserStatsButton);

  layout->addLayout(dropDownLayout);

  username = "N/A";
  usernameTitle = new QLabel(username);
  layout->addWidget(usernameTitle);

  QHBoxLayout* statisticsLayout = new QHBoxLayout();
  QVBoxLayout* gamesLayout = new QVBoxLayout();
  QVBoxLayout* guessLayout = new QVBoxLayout();

  QHBoxLayout* gamesPlayedLayout = new QHBoxLayout();
  gamesPlayedLayout->addWidget(new QLabel("Games Played:"));
  gamesPlayedStats = new QLabel("N/A");
  gamesPlayedLayout->addWidget(gamesPlayedStats);

  QHBoxLayout* gamesWinLayout = new QHBoxLayout();
  gamesWinLayout->addWidget(new QLabel("Games Won:"));
  gamesWinStats = new QLabel("N/A");
  gamesWinLayout->addWidget(gamesWinStats);

  QHBoxLayout* gamesWinRateLayout = new QHBoxLayout();
  gamesWinRateLayout->addWidget(new QLabel("Games Win Rate:"));
  gamesWinRateStats = new QLabel("N/A");
  gamesWinRateLayout->addWidget(gamesWinRateStats);

  gamesLayout->addLayout(gamesPlayedLayout);
  gamesLayout->addLayout(gamesWinLayout);
  gamesLayout->addLayout(gamesWinRateLayout);

  QHBoxLayout* guessTotalLayout = new QHBoxLayout();
  guessTotalLayout->addWidget(new QLabel("Guess Total:"));
  guessTotalStats = new QLabel("N/A");
  guessTotalLayout->addWidget(guessTotalStats);

  QHBoxLayout* guessHitLayout = new QHBoxLayout();
  guessHitLayout->addWidget(new QLabel("Guess Hit:"));
  guessHitStats = new QLabel("N/A");
  guessHitLayout->addWidget(guessHitStats);

  QHBoxLayout* guessHitRateLayout = new QHBoxLayout();
  guessHitRateLayout->addWidget(new QLabel("Guess Hit Rate:"));
  guessHitRateStats = new QLabel("N/A");
  guessHitRateLayout->addWidget(guessHitRateStats);

  guessLayout->addLayout(guessTotalLayout);
  guessLayout->addLayout(guessHitLayout);
  guessLayout->addLayout(guessHitRateLayout);

  statisticsLayout->addLayout(gamesLayout);
  statisticsLayout->addLayout(guessLayout);
  layout->addLayout(statisticsLayout);

  setLayout(layout);

  populateDropDown();

  connect(showUserStatsButton, &QPushButton::clicked, this,
          &StatisticsWindow::showUserStats);
  connect(backToMainButton, &QPushButton::clicked, this,
          &StatisticsWindow::goBackToMain);
}

StatisticsWindow::~StatisticsWindow() {}

void StatisticsWindow::populateDropDown() {
  QJsonObject json = users->loadJsonFile();
  QStringList usernames = json.keys();

  usernameComboBox->clear();
  usernameComboBox->addItems(usernames);
}

void StatisticsWindow::showUserStats() {
  username = usernameComboBox->currentText().trimmed();

  usernameTitle->setText(username);

  gamesPlayedStats->setText(QString::number(users->getGamesPlayed(username)));
  gamesWinStats->setText(QString::number(users->getWins(username)));

  // Specify decimal places for floating-point numbers
  gamesWinRateStats->setText(
      QString::number(users->getWinRate(username) * 100, 'f', 2) + "%");
  guessTotalStats->setText(QString::number(users->getGuessTotal(username)));
  guessHitStats->setText(QString::number(users->getGuessHit(username)));
  guessHitRateStats->setText(
      QString::number(users->getHitRate(username) * 100, 'f', 2) + "%");
}

void StatisticsWindow::goBackToMain() {
  this->hide();
  emit backToMainWindow();
}

void StatisticsWindow::show() {
  QWidget::show();
  qDebug() << "Statistics shown";
}