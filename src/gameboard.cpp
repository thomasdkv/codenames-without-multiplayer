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
    setFixedSize(1000, 800);
    
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
    currentTurnLabel = new QLabel("Current Turn: " + redSpyMasterName);
        
    mainLayout->addWidget(redTeamLabel);
    mainLayout->addWidget(blueTeamLabel);
    mainLayout->addWidget(currentTurnLabel);

    // Display the current hint (initially empty) above the grid
    currentHint = new QLabel("Current hint: ");
    currentHint->setAlignment(Qt::AlignCenter);
    currentHint->setStyleSheet("font-weight: bold; font-size: 20px; color: black; ");
    mainLayout->insertWidget(2, currentHint);

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
                    cards[i][j]->setStyleSheet("background-color: #ff9999; color: black");
                    break;
                case BLUE_TEAM:
                    cards[i][j]->setStyleSheet("background-color: #9999ff; color: black");
                    break;
                case NEUTRAL:
                    cards[i][j]->setStyleSheet("background-color: #f0f0f0; color: black");
                    break;
                case ASSASSIN:
                    cards[i][j]->setStyleSheet("background-color: #333333; color: white;");
                    break;
            }
            cards[i][j]->setEnabled(false);
            // Connect the button's clicked signal to a lambda or a slot
            connect(cards[i][j], &QPushButton::clicked, this, [=]() {
                onCardClicked(i, j);
            });
        }
    }
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    // Implement the transition widget
    transition = new Transition(this);
    transition->setFixedSize(400, 200);
    transition->hide();
    mainLayout->addWidget(transition);
    connect(transition, &Transition::continueClicked, this, &GameBoard::onContinueClicked);
    
    // Implement the Spymaster hint widget and connect the hintSubmitted signal to displayHint slot
    spymasterHint = new SpymasterHint(this);
    mainLayout->addWidget(spymasterHint);
    connect(spymasterHint, &SpymasterHint::hintSubmitted, this, &GameBoard::displayHint);

    // Implement Operator guess (start hidden)
    operatorGuess = new OperatorGuess(this);
    mainLayout->addWidget(operatorGuess);
    operatorGuess->setVisible(false);
    connect(operatorGuess, &OperatorGuess::guessSubmitted, this, &GameBoard::displayGuess);
}

void GameBoard::displayGuess() {
    nextTurn();
    showTransition();
}

void GameBoard::onCardClicked(int row, int col) {
    if (gameGrid[row][col].revealed) {
        return;
    }
    gameGrid[row][col].revealed = true;
    cards[row][col]->setText("");
    cards[row][col]->setEnabled(false);
    if(currentTurn == RED_OP && gameGrid[row][col].type == RED_TEAM || currentTurn == BLUE_OP && gameGrid[row][col].type == BLUE_TEAM) {
        switch (gameGrid[row][col].type) {
            case RED_TEAM:
                cards[row][col]->setStyleSheet("background-color: #ff9999; color: black");
                break;
            case BLUE_TEAM:
                cards[row][col]->setStyleSheet("background-color: #9999ff; color: black");
                break;
        }
        cards[row][col]->setEnabled(false);
    }
    else {
        nextTurn();
    }

    // Check if the card clicked is a correct card
    bool correctCard = (currentTurn == RED_OP && gameGrid[row][col].type == RED_TEAM) || 
                       (currentTurn == BLUE_OP && gameGrid[row][col].type == BLUE_TEAM);

    // If the card clicked is not a correct card, show the transition widget
    if (!correctCard) {
        showTransition();
    }
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

    nextTurn();
}

void GameBoard::nextTurn() {
    // Switch to the next player's turn
    currentTurn = (currentTurn + 1) % 4;

    // Hide Board for next player
    if (currentTurn == RED_OP || currentTurn == BLUE_OP) {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if(gameGrid[i][j].revealed == false) {
                    cards[i][j]->setEnabled(true);
                    cards[i][j]->setStyleSheet("background-color: #f0f0f0; color: black");
                }
            }
        }
        // Remove spymaster widget
        spymasterHint->hide();
        operatorGuess->setVisible(true);
    }

    // Reveal the board for spymaster
    if (currentTurn == RED_SPY || currentTurn == BLUE_SPY) {
        currentHint->setText("Current hint: "); // Clear the hint
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                cards[i][j]->setEnabled(false);
                switch (gameGrid[i][j].type) {
                    case RED_TEAM:
                        cards[i][j]->setStyleSheet("background-color: #ff9999; color: black");
                        break;
                    case BLUE_TEAM:
                        cards[i][j]->setStyleSheet("background-color: #9999ff; color: black");
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
        // Show spymaster widget
        spymasterHint->show();
        operatorGuess->setVisible(false);
    }

    // Update the team labels
    if(currentTurn == RED_SPY) {
        currentTurnLabel->setText("Current Turn: " + redSpyMasterName);
    }
    else if(currentTurn == RED_OP) {
        currentTurnLabel->setText("Current Turn: " + redOperativeName);
    }
    else if(currentTurn == BLUE_SPY) {
        currentTurnLabel->setText("Current Turn: " + blueSpyMasterName);
    }
    else if(currentTurn == BLUE_OP) {
        currentTurnLabel->setText("Current Turn: " + blueOperativeName);
    }

}

void GameBoard::onContinueClicked() {
    transition->hide();
    spymasterHint->setEnabled(true);
    operatorGuess->setEnabled(true);

    if (currentTurn == RED_OP || currentTurn == BLUE_OP) {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (!gameGrid[i][j].revealed) {
                    cards[i][j]->setStyleSheet("background-color: #f0f0f0; color: black");
                    cards[i][j]->setEnabled(true);
                } 
            }
        }
    }
    nextTurn();
}

void GameBoard::showTransition() {
    // Determine the next player's turn and name
    int nextPlayerTurn = (currentTurn + 1) % 4;
    QString nextSpymasterName;

    // Skip to the next spymaster
    while (nextPlayerTurn != RED_SPY && nextPlayerTurn != BLUE_SPY) {
        nextPlayerTurn = (nextPlayerTurn + 1) % 4;
    }

    // Set the next spymaster's name
    if (nextPlayerTurn == RED_SPY) {
        nextSpymasterName = blueSpyMasterName;
    } else if (nextPlayerTurn == BLUE_SPY) {
        nextSpymasterName = redSpyMasterName;
    }

    // Disable all elements 
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cards[i][j]->setEnabled(false);
            if(gameGrid[i][j].revealed == false) {
                cards[i][j]->setStyleSheet("background-color: #f0f0f0; color: black");
            }
        }
    }
    spymasterHint->setEnabled(false);
    operatorGuess->setEnabled(false);

    currentTurn = (nextPlayerTurn + 1) % 4; // Skip the next spymaster and go to the next operative

    transition->setMessage("Please pass the device to " + nextSpymasterName);
    transition->show();
}