#ifndef MULTIBOARD_H
#define MULTIBOARD_H

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

private slots:
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

    // Player information
    QHash<QString, QString> m_playerRoles;
    QString m_currentUsername;
    QString m_currentRole;

    // Game board components
    QGridLayout* m_grid;
    QLabel* m_playerInfoLabel;
    QLabel* m_turnLabel;
    QList<QPushButton*> m_tiles;

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
    
    void revealTile(int index, bool broadcast = true);
    void advanceTurn();
    void updateTurnDisplay();
    void sendToAll(const QString& message);

    // Utility methods
    bool isMyTurn() const;
    QString getMyTeam() const;
    QString getColorStyle(const QString& color) const;
};

#endif // MULTIBOARD_H