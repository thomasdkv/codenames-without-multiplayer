#ifndef MULTIBOARD_H
#define MULTIBOARD_H


#include "../spymasterhint.h"
#include "../operatorguess.h"
#include "user.h"
#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QFile>

class MultiBoard : public QWidget 
{
    Q_OBJECT

public:
    explicit MultiBoard(
        bool isHost, 
        QWebSocketServer* server, 
        QList<QWebSocket*> clients,
        QWebSocket* clientSocket, 
        const QHash<QString, QString>& playerRoles,
        const QString& currentUsername, 
        QWidget* parent = nullptr
    );
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

public slots:
    void handleTileClick();
    void processMessage(const QString& message);
    void socketDisconnected();
    void handleNewConnection();

private:
    // Network setup
    bool m_isHost;
    QWebSocketServer* m_server;
    QList<QWebSocket*> m_clients;
    QWebSocket* m_clientSocket;

    User* users;

    // Player information
    QHash<QString, QString> m_playerRoles;
    QString m_currentUsername;
    QString m_currentRole;

    // Game board components
    QVBoxLayout* mainLayout;
    QGridLayout* m_grid;
    QLabel* m_playerInfoLabel;
    QLabel* m_turnLabel;
    QList<QPushButton*> m_tiles;
    SpymasterHint* hint;
    OperatorGuess* guess;


    // Game state
    QStringList m_words;
    QStringList m_tileColors;
    QStringList m_turnOrder;
    int m_currentTurnIndex;

    // Private methods
    void setupUI();
    void setupBoard();
    void initializeWords();
    void initializeBoardColors();
    void sendInitialGameState();
    void loadWordsFromFile();
    void generateGameGrid();
    void checkGameEnd();
    
    void revealTile(int row, int col, bool broadcast = true);
    void advanceTurn();
    void advanceTurnSpymaster(const QString& hint, int number);
    void updateTurnDisplay();
    void sendToAll(const QString& message);
    void displayHint(const QString& hint, int number);

    static const int GRID_SIZE = 5;
    Card gameGrid[GRID_SIZE][GRID_SIZE];
    QStringList wordList;
    QPushButton* cards[GRID_SIZE][GRID_SIZE];
    QLabel* currentHint;
    QString correspondingNumber;

    // Utility methods
    bool isMyTurn() const;
    QString getMyTeam() const;
    QString getColorStyle(const QString& color) const;
};

#endif // MULTIBOARD_H