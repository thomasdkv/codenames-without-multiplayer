#ifndef MULTIPREGAME_H
#define MULTIPREGAME_H

#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QLoggingCategory>

class MultiPregame : public QWidget {
    Q_OBJECT
public:
    // Host constructor
    MultiPregame(QWebSocketServer* server, const QString& username, QWidget* parent = nullptr);
    // Client constructor
    MultiPregame(QWebSocket* socket, const QString& username, QWidget* parent = nullptr);
    ~MultiPregame();
    void clearUI();


signals:
    void backToMultiMain();
    void enterPregameAsHost(QWebSocketServer* server, const QString& username);
    void enterPregameAsClient(QWebSocket* socket, const QString& username);

public slots:
    void onNewConnection();
    void processMessage(const QString& message);
    void socketDisconnected();
    void startGame();

private:
    void resetUIState();
    void setupUI();
    void sendLobbyUpdate();
    void handleRoleSelection(const QString& message, QWebSocket* sender);
    void gameStarted(bool isHost, QWebSocketServer* server, const QList<QWebSocket*>& clients, QWebSocket* clientSocket, const QHash<QString, QString>& playerRoles);
    void showPregame();
    QWebSocketServer* m_server = nullptr;
    QWebSocket* m_clientSocket = nullptr;
    QList<QWebSocket*> m_clients;
    QMap<QWebSocket*, QString> m_usernames;
    QMap<QWebSocket*, QString> m_roles;
    QMap<QWebSocket*, bool> m_checked;
    
    QListWidget* playerList;
    QString m_username;
    bool m_isHost;

};

#endif