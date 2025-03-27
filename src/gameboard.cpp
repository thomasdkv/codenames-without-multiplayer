#include "gameboard.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>

GameBoard::GameBoard(const QString& redSpyMaster, const QString& redOperative,
                    const QString& blueSpyMaster, const QString& blueOperative,
                    QWidget* parent)
    : QWidget(parent),
      redSpyMasterName(redSpyMaster),
      redOperativeName(redOperative),
      blueSpyMasterName(blueSpyMaster),
      blueOperativeName(blueOperative), 
      
      redCardsRemaining(0),
      blueCardsRemaining(0)
{
    
    setWindowTitle("Codenames - Game Board");
    setFixedSize(1000, 800);
    
    loadWordsFromFile();
    generateGameGrid();
    setupUI();
}

GameBoard::~GameBoard() {}

void GameBoard::show() {
    resetGame();
    QWidget::show();
    qDebug() << "GameBoard shown";
}

void GameBoard::setRedSpyMasterName(const QString& name) {
    redSpyMasterName = name;
}

void GameBoard::setRedOperativeName(const QString& name) {
    redOperativeName = name;
}

void GameBoard::setBlueSpyMasterName(const QString& name) {
    blueSpyMasterName = name;
}

void GameBoard::setBlueOperativeName(const QString& name) {
    blueOperativeName = name;
}

void GameBoard::updateTeamLabels() {
    redTeamLabel->setText("Red Team - Spymaster: " + redSpyMasterName + ", Operative: " + redOperativeName);
    blueTeamLabel->setText("Blue Team - Spymaster: " + blueSpyMasterName +  ", Operative: " + blueOperativeName);
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

    redCardsRemaining = redCards;
    blueCardsRemaining = blueCards;

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
        
    redScoreLabel = new QLabel("Red Cards Remaining: " + QString::number(redCardsRemaining));
    blueScoreLabel = new QLabel("Blue Cards remaining: " + QString::number(blueCardsRemaining));

    // Create a horizontal layout for score labels to place them side by side
    QHBoxLayout* scoreLayout = new QHBoxLayout();
    scoreLayout->addWidget(redScoreLabel);
    scoreLayout->addStretch();
    scoreLayout->addWidget(blueScoreLabel);
    mainLayout->addLayout(scoreLayout);

    // Create a horizontal layout for team labels to place them side by side
    QHBoxLayout* teamLayout = new QHBoxLayout();
    teamLayout->addWidget(redTeamLabel);
    teamLayout->addStretch();
    teamLayout->addWidget(blueTeamLabel);
    mainLayout->addLayout(teamLayout);

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

    // Mark the card as revealed and disable it
    gameGrid[row][col].revealed = true;
    cards[row][col]->setText("");  // Clear the text to show the card is revealed
    cards[row][col]->setEnabled(false);

    // Always reveal the card's true color, regardless of whether it's correct
    switch (gameGrid[row][col].type) {
        case RED_TEAM:
            cards[row][col]->setStyleSheet("background-color: #ff9999; color: black");
            break;
        case BLUE_TEAM:
            cards[row][col]->setStyleSheet("background-color: #9999ff; color: black");
            break;
        case NEUTRAL:
            cards[row][col]->setStyleSheet("background-color: #f0f0f0; color: black");
            break;
        case ASSASSIN:
            cards[row][col]->setStyleSheet("background-color: #333333; color: white");
            break;
    }

    // Update scores 
    if (gameGrid[row][col].type == RED_TEAM) {
        redCardsRemaining--;
        updateScores();
        qDebug() << "Red team card selected. Red cards remaining:" << redCardsRemaining;
    } else if (gameGrid[row][col].type == BLUE_TEAM) {
        blueCardsRemaining--;
        updateScores();
        qDebug() << "Blue team card selected. Blue cards remaining:" << blueCardsRemaining;
    }

    bool correctCard = (currentTurn == RED_OP && gameGrid[row][col].type == RED_TEAM) || 
                       (currentTurn == BLUE_OP && gameGrid[row][col].type == BLUE_TEAM);

    checkGameEnd();

    // Show the transition widget
    if (!correctCard) {
        qDebug() << "Wrong card selected by" << (currentTurn == RED_OP ? "Red team" : "Blue team")
                 << "- Card type:" << gameGrid[row][col].type;
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                cards[i][j]->setEnabled(false);
            }
        }
        operatorGuess->setVisible(false);

        nextTurn();
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
                if (!gameGrid[i][j].revealed) {
                    cards[i][j]->setEnabled(true);
                    cards[i][j]->setStyleSheet("background-color: #f0f0f0; color: black");
                } 
                else {
                    cards[i][j]->setEnabled(false);  // Ensure revealed cards stay disabled
                }
            }
        }
        // Remove spymaster widget
        spymasterHint->hide();
        operatorGuess->setVisible(true);
        operatorGuess->setEnabled(true);
        operatorGuess->reset();
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

    if (currentTurn == RED_SPY || currentTurn == BLUE_SPY) {
        currentHint->setText("Current hint: ");
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
                        cards[i][j]->setStyleSheet("background-color: #333333; color: white");
                        break;
                }            
            }
        }
        spymasterHint->setEnabled(true);
        spymasterHint->show();
        operatorGuess->setEnabled(false);
        operatorGuess->setVisible(false);
        qDebug() << "Spymaster turn - Cards disabled, showing hint widget";
    }

    // Update the current turn label
    if (currentTurn == RED_SPY) {
        currentTurnLabel->setText("Current Turn: " + redSpyMasterName);
    } else if (currentTurn == BLUE_SPY) {
        currentTurnLabel->setText("Current Turn: " + blueSpyMasterName);
    }

    qDebug() << "Continue clicked, spymaster turn set up. Current turn:" << currentTurn;
}

void GameBoard::showTransition() {
    QString nextSpymasterName;

    if (currentTurn == RED_SPY) {
        nextSpymasterName = redSpyMasterName;
    } 
    else if (currentTurn == BLUE_SPY) {
        nextSpymasterName = blueSpyMasterName;
    } 
    else {
        qDebug() << "Error: showTransition called when currentTurn is not a spymaster turn:" << currentTurn;
        return;
    }

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
    // I'm not sure if we want to show spymasterhint and operatorguess on transition screen. I thought it would be nicer
    // if it didn't show.
    spymasterHint->setVisible(false);
    operatorGuess->setVisible(false);

    transition->setMessage("Please pass the device to " + nextSpymasterName);
    transition->show();
}

void GameBoard::updateScores() {
    redScoreLabel->setText("Red Cards Remaining: " + QString::number(redCardsRemaining));
    blueScoreLabel->setText("Blue Cards Remaining: " + QString::number(blueCardsRemaining));
}

void GameBoard::checkGameEnd() {
    if (redCardsRemaining == 0) {
        endGame("Red Team Wins!");
        return;
    }
    if (blueCardsRemaining == 0) {
        endGame("Blue Team Wins!");
        return;
    }

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (gameGrid[i][j].revealed && gameGrid[i][j].type == ASSASSIN) {
                if (currentTurn == RED_OP) {
                    endGame("Blue Team Wins! Red Team hit the Assassin card.");
                } else if (currentTurn == BLUE_OP) {
                    endGame("Red Team Wins! Blue Team hit the Assassin card.");
                }
                return;
            }
        }
    }
}

void GameBoard::endGame(const QString& message) {
    // Disable all elements
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cards[i][j]->setEnabled(false);
        }
    }
    spymasterHint->setEnabled(false);
    operatorGuess->setEnabled(false);
    transition->hide();

    // Show a pop-up with the game result and an "OK" button
    QMessageBox endGameBox;
    endGameBox.setWindowTitle("Game Over");
    endGameBox.setText(message);
    endGameBox.setStandardButtons(QMessageBox::Ok);
    endGameBox.setDefaultButton(QMessageBox::Ok);
    endGameBox.exec();

    // Emit the gameEnded signal to notify PreGame to return to the main menu
    emit gameEnded();

    this->close();
}

void GameBoard::resetGame() {

    loadWordsFromFile();
    generateGameGrid();

    // Reset the UI elements
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cards[i][j]->setText(gameGrid[i][j].word);
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

    // Reset scores
    redCardsRemaining = 0;
    blueCardsRemaining = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (gameGrid[i][j].type == RED_TEAM) redCardsRemaining++;
            if (gameGrid[i][j].type == BLUE_TEAM) blueCardsRemaining++;
        }
    }
    updateScores();

    // Reset turn and labels
    currentTurn = RED_SPY;
    currentTurnLabel->setText("Current Turn: " + redSpyMasterName);
    currentHint->setText("Current hint: ");

    // Reset widget states
    spymasterHint->setEnabled(true); 
    spymasterHint->show();

    spymasterHint->reset();

    operatorGuess->setEnabled(true);
    operatorGuess->setVisible(false);

    operatorGuess->reset();

    transition->hide();
}