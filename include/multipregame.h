#ifndef MULTIPREGAME_H
#define MULTIPREGAME_H

#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QListWidget>

class MultiPregame : public QWidget {
    Q_OBJECT
public:
    // Host constructor
    MultiPregame(QWebSocketServer* server, const QString& username, QWidget* parent = nullptr);
    // Client constructor
    MultiPregame(QWebSocket* socket, const QString& username, QWidget* parent = nullptr);
    ~MultiPregame();

signals:
    void backToMultiMain();
    void enterPregameAsHost(QWebSocketServer* server, const QString& username);
    void enterPregameAsClient(QWebSocket* socket, const QString& username);

private slots:
    void onNewConnection();
    void processMessage(const QString& message);
    void socketDisconnected();
    void startGame();

private:
    void setupUI();
    void sendLobbyUpdate();
    void handleRoleSelection(const QString& message, QWebSocket* sender);

    QWebSocketServer* m_server = nullptr;
    QWebSocket* m_clientSocket = nullptr;
    QList<QWebSocket*> m_clients;
    QMap<QWebSocket*, QString> m_usernames;
    QMap<QWebSocket*, QString> m_roles;
    
    QListWidget* playerList;
    QString m_username;
    bool m_isHost;
};

#endif