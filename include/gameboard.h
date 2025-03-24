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

class GameBoard : public QWidget {
    Q_OBJECT

public:
    explicit GameBoard(const QString& redSpyMaster, const QString& redOperative,
                      const QString& blueSpyMaster, const QString& blueOperative,
                      QWidget* parent = nullptr);
    ~GameBoard();

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
};

#endif // GAMEBOARD_H