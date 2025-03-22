#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QStringList>
#include <QPushButton>
#include "spymasterhint.h"

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

private:
    void loadWordsFromFile();
    void generateGameGrid();
    void setupUI();

    enum CardType {
        RED_TEAM,
        BLUE_TEAM,
        NEUTRAL,
        ASSASSIN
    };

    struct Card {
        QString word;
        CardType type;
        bool revealed;
    };

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

    SpymasterHint* spymasterHint;
    QLabel* currentHint;
    QLabel* correspondingNumber;
};

#endif // GAMEBOARD_H