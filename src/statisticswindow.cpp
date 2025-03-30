/**
 * @file statisticswindow.cpp
 * @author Team 9 - UWO CS 3307
 * @brief The screen to show the user's statistics
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "statisticswindow.h"

StatisticsWindow::StatisticsWindow(QWidget* parent) : QWidget(parent) {
  this->setFixedSize(1000, 800);

  // Center the window on the screen
  QScreen* screen = QGuiApplication::primaryScreen();
  if (screen) {
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
  }

  users = User::instance();

  QVBoxLayout* layout = new QVBoxLayout(this);

  QString buttonStyles =
      "QPushButton {"
      "   background-color:rgb(65, 42, 213);"
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

  QString comboBoxStyle =
      "QComboBox {"
      "   background-color: #2a2a2a;"
      "   color: white;"
      "   border-radius: 5px;"
      "   border: 2px solid #412AD5;"
      "   padding: 5px;"
      "   font-size: 16px;"
      "}"
      "QComboBox::drop-down {"
      "   background-color: #2a2a2a;"
      "   border: 2px solid #412AD5;"
      "}"
      "QComboBox QAbstractItemView {"
      "   background-color: #2a2a2a;"
      "   color: white;"
      "   border: 2px solid #412AD5;"
      "   selection-background-color: rgb(54, 35, 177);"
      "}";

  QString statsStyle = "color: white; font-size: 18px;";

  // Back to Main Button Styling
  backToMainButton = new QPushButton("Back to Main Menu", this);
  backToMainButton->setFixedSize(220, 50);
  backToMainButton->setStyleSheet(buttonStyles);
  layout->addWidget(backToMainButton);

  QHBoxLayout* dropDownLayout = new QHBoxLayout();

  // ComboBox Styling
  usernameComboBox = new QComboBox();
  usernameComboBox->setStyleSheet(comboBoxStyle);
  dropDownLayout->addWidget(usernameComboBox);

  // Show Stats Button Styling
  showUserStatsButton = new QPushButton("Show Stats", this);
  showUserStatsButton->setFixedSize(200, 50);
  showUserStatsButton->setStyleSheet(buttonStyles);
  dropDownLayout->addWidget(showUserStatsButton);
  layout->addLayout(dropDownLayout);

  // Username Label Styling
  usernameTitle = new QLabel(username, this);
  usernameTitle->setAlignment(Qt::AlignCenter);
  usernameTitle->setStyleSheet(
      "font-size: 24px; color: white; margin: 10px; font-weight: bold;");
  layout->addWidget(usernameTitle);

  QHBoxLayout* statisticsLayout = new QHBoxLayout();
  QVBoxLayout* gamesLayout = new QVBoxLayout();
  QVBoxLayout* guessLayout = new QVBoxLayout();

  // Stats Layout Styling
  QHBoxLayout* gamesPlayedLayout = new QHBoxLayout();
  gamesPlayedLayout->addWidget(new QLabel("Games Played:", this));
  gamesPlayedStats = new QLabel("N/A", this);
  gamesPlayedStats->setStyleSheet(statsStyle);
  gamesPlayedLayout->addWidget(gamesPlayedStats);

  QHBoxLayout* gamesWinLayout = new QHBoxLayout();
  gamesWinLayout->addWidget(new QLabel("Games Won:", this));
  gamesWinStats = new QLabel("N/A", this);
  gamesWinStats->setStyleSheet(statsStyle);
  gamesWinLayout->addWidget(gamesWinStats);

  QHBoxLayout* gamesWinRateLayout = new QHBoxLayout();
  gamesWinRateLayout->addWidget(new QLabel("Games Win Rate:", this));
  gamesWinRateStats = new QLabel("N/A", this);
  gamesWinRateStats->setStyleSheet(statsStyle);
  gamesWinRateLayout->addWidget(gamesWinRateStats);

  gamesLayout->addLayout(gamesPlayedLayout);
  gamesLayout->addLayout(gamesWinLayout);
  gamesLayout->addLayout(gamesWinRateLayout);

  QHBoxLayout* guessTotalLayout = new QHBoxLayout();
  guessTotalLayout->addWidget(new QLabel("Guess Total:", this));
  guessTotalStats = new QLabel("N/A", this);
  guessTotalStats->setStyleSheet(statsStyle);
  guessTotalLayout->addWidget(guessTotalStats);

  QHBoxLayout* guessHitLayout = new QHBoxLayout();
  guessHitLayout->addWidget(new QLabel("Guess Hit:", this));
  guessHitStats = new QLabel("N/A", this);
  guessHitStats->setStyleSheet(statsStyle);
  guessHitLayout->addWidget(guessHitStats);

  QHBoxLayout* guessHitRateLayout = new QHBoxLayout();
  guessHitRateLayout->addWidget(new QLabel("Guess Hit Rate:", this));
  guessHitRateStats = new QLabel("N/A", this);
  guessHitRateStats->setStyleSheet(statsStyle);
  guessHitRateLayout->addWidget(guessHitRateStats);

  guessLayout->addLayout(guessTotalLayout);
  guessLayout->addLayout(guessHitLayout);
  guessLayout->addLayout(guessHitRateLayout);

  gamesLayout->setAlignment(Qt::AlignHCenter);
  guessLayout->setAlignment(Qt::AlignHCenter);

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
  populateDropDown();
  qDebug() << "Statistics shown";
}