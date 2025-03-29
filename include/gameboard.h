#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QStringList>
#include <QPushButton>
#include <QStackedLayout>
#include "spymasterhint.h"
#include "operatorguess.h"
#include "transition.h"
#include "chatbox.h"
#include "user.h"

class GameBoard : public QWidget {
    Q_OBJECT

public:
    explicit GameBoard(const QString& redSpyMaster, const QString& redOperative,
                      const QString& blueSpyMaster, const QString& blueOperative,
                      QWidget* parent = nullptr);
    ~GameBoard();

    void setRedSpyMasterName(const QString& name);
    void setRedOperativeName(const QString& name);
    void setBlueSpyMasterName(const QString& name);
    void setBlueOperativeName(const QString& name);
    void updateTeamLabels();

signals:
    void gameEnded();

public slots:
    void show();
    void displayHint(const QString& hint, int number);
    void displayGuess();

private:
    void loadWordsFromFile();
    void generateGameGrid();
    void setupUI();
    void nextTurn();
    void onCardClicked(int row, int col);
    void onContinueClicked();
    void showTransition();

    void updateScores();
    void checkGameEnd();
    void endGame(const QString& message);

    void resetGame();

    enum CardType {
        RED_TEAM,
        BLUE_TEAM,
        NEUTRAL,
        ASSASSIN
    };

    enum Turn {
        RED_SPY,
        RED_OP,
        BLUE_SPY,
        BLUE_OP
    };

    struct Card {
        QString word;
        CardType type;
        bool revealed;
    };

    int currentTurn;
    int redCardsRemaining;
    int blueCardsRemaining;

    int maxGuesses=0;
    int currentGuesses=0;

    QString redSpyMasterName;
    QString redOperativeName;
    QString blueSpyMasterName;
    QString blueOperativeName;

    static const int GRID_SIZE = 5;
    Card gameGrid[GRID_SIZE][GRID_SIZE];
    QStringList wordList;
    

    
    QGridLayout* gridLayout;
    QPushButton* cards[GRID_SIZE][GRID_SIZE];
    
    QLabel* redTeamLabel;
    QLabel* blueTeamLabel;
    QLabel* currentTurnLabel;

    SpymasterHint* spymasterHint;
    OperatorGuess* operatorGuess;
    QLabel* currentHint;
    QString correspondingNumber;

    Transition* transition;

    QLabel* redScoreLabel;
    QLabel* blueScoreLabel;

    ChatBox* chatBox;
    QString currentPlayerName;
    ChatBox::Team currentPlayerTeam;
    User* users;
};

#endif // GAMEBOARD_H