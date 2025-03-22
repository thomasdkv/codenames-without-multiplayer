#include "gameboard.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QDebug>

GameBoard::GameBoard(const QString& redSpyMaster, const QString& redOperative,
                    const QString& blueSpyMaster, const QString& blueOperative,
                    QWidget* parent)
    : QWidget(parent),
      redSpyMasterName(redSpyMaster),
      redOperativeName(redOperative),
      blueSpyMasterName(blueSpyMaster),
      blueOperativeName(blueOperative) {
    
    setWindowTitle("Codenames - Game Board");
    setFixedSize(800, 600);
    
    loadWordsFromFile();
    generateGameGrid();
    setupUI();
}

GameBoard::~GameBoard() {}

void GameBoard::show() {
    QWidget::show();
    qDebug() << "GameBoard shown";
}

void GameBoard::loadWordsFromFile() {
    QFile file(":/resources/wordlist-eng.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "failed to open wordlist-eng.text file, using fallback";
        // Fallback word list if file not found
        wordList = QStringList() << "apple" << "banana" << "cat" << "dog" << "elephant"
                                << "fish" << "goat" << "horse" << "iguana" << "jelly"
                                << "kangaroo" << "lion" << "monkey" << "nest" << "owl"
                                << "penguin" << "queen" << "rabbit" << "snake" << "tiger"
                                << "umbrella" << "violin" << "whale" << "xray" << "zebra";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            wordList.append(line);
        }
    }
    qDebug() << "Loaded" << wordList.size() << "words";
    file.close();
}

void GameBoard::generateGameGrid() {
    // Ensure we have enough words
    if (wordList.size() < 25) {
        qDebug() << "Not enough words to generate a game grid" << wordList.size();
        return;
    }

    // Shuffle words
    for (int i = wordList.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        wordList.swapItemsAt(i, j);
    }

    // Assign card types (Codenames rules: 9 for starting team, 8 for other team, 1 assassin, 7 neutral)
    bool redStarts = QRandomGenerator::global()->bounded(2) == 0;
    int redCards = redStarts ? 9 : 8;
    int blueCards = redStarts ? 8 : 9;
    int assassinCards = 1;
    int neutralCards = 7;

    QList<CardType> cardTypes;
    for (int i = 0; i < redCards; ++i) cardTypes.append(RED_TEAM);
    for (int i = 0; i < blueCards; ++i) cardTypes.append(BLUE_TEAM);
    for (int i = 0; i < neutralCards; ++i) cardTypes.append(NEUTRAL);
    for (int i = 0; i < assassinCards; ++i) cardTypes.append(ASSASSIN);

    // Shuffle card types
    for (int i = cardTypes.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        cardTypes.swapItemsAt(i, j);
    }

    // Fill the grid
    int wordIndex = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            gameGrid[i][j].word = wordList[wordIndex];
            gameGrid[i][j].type = cardTypes[wordIndex];
            gameGrid[i][j].revealed = false;
            ++wordIndex;
        }
    }
}

void GameBoard::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Team info
    redTeamLabel = new QLabel("Red Team - Spymaster: " + redSpyMasterName + 
                            ", Operative: " + redOperativeName);
    blueTeamLabel = new QLabel("Blue Team - Spymaster: " + blueSpyMasterName + 
                             ", Operative: " + blueOperativeName);
    
    mainLayout->addWidget(redTeamLabel);
    mainLayout->addWidget(blueTeamLabel);

    // Grid setup
    gridLayout = new QGridLayout();
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cards[i][j] = new QPushButton(gameGrid[i][j].word);
            qDebug() << "card at" << i << j << "set to" << gameGrid[i][j].word;
            cards[i][j]->setFixedSize(120, 80);
            gridLayout->addWidget(cards[i][j], i, j);
            
            // For now, we'll show the card type in the background (for testing)
            // In a real game, this would be hidden from operatives
            switch (gameGrid[i][j].type) {
                case RED_TEAM:
                    cards[i][j]->setStyleSheet("background-color: #ff9999;");
                    break;
                case BLUE_TEAM:
                    cards[i][j]->setStyleSheet("background-color: #9999ff;");
                    break;
                case NEUTRAL:
                    cards[i][j]->setStyleSheet("background-color: #f0f0f0; color: black");
                    break;
                case ASSASSIN:
                    cards[i][j]->setStyleSheet("background-color: #333333; color: white;");
                    break;
            }
        }
    }
    
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    // Display the current hint (initially empty) above the grid
    currentHint = new QLabel("Current hint: ");
    currentHint->setAlignment(Qt::AlignCenter);
    currentHint->setStyleSheet("font-weight: bold; font-size: 20px; color: black; ");
    mainLayout->insertWidget(2, currentHint);

    // Implement the Spymaster hint widget and connect the hintSubmitted signal to displayHint slot
    spymasterHint = new SpymasterHint(this);
    mainLayout->addWidget(spymasterHint);
    connect(spymasterHint, &SpymasterHint::hintSubmitted, this, &GameBoard::displayHint);
}

void GameBoard::displayHint(const QString& hint, int number) {
    qDebug() << "Received hint:" << hint << "for" << number << "corresponding words";

    // Update the coreesponding number for the hint, if it is 0, display "∞"
    if (number == 0) {
        correspondingNumber = "∞";
    } else {
        correspondingNumber = QString::number(number);
    }
    currentHint->setText("Current hint: " + hint + " (" + correspondingNumber + ")"); // Update the hint
}