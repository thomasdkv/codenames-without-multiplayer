#ifndef MULTIMAIN_H
#define MULTIMAIN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QMap>

class MultiMain : public QWidget
{
    Q_OBJECT
public:
    explicit MultiMain(QWidget *parent = nullptr);
    ~MultiMain();

    void showMainWindow();

signals:
    void backToMainWindow();
    void enterPregameAsHost(QWebSocketServer* server, const QString& username);
    void enterPregameAsClient(QWebSocket* socket, const QString& username);
private slots:
    // Button click handlers
    void openMainWindow();
    void onCreateRoomClicked();
    void onJoinRoomClicked();

    // Network handlers
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
    void onConnected();
    void onDisconnected();

private:
    // Network members
    QWebSocketServer *m_server = nullptr;
    QWebSocket *m_clientSocket = nullptr;
    QList<QWebSocket*> m_clients;
    QMap<QWebSocket*, QString> m_usernames;
    QString m_username;

    // UI members
    QLabel *titleLabel;
    QPushButton *createRoomButton;
    QPushButton *joinRoomButton;
    QPushButton *backButton;

    // Lobby management
    void updateLobbyList();
    void sendLobbyListToAll();
};

#endif // MULTIMAIN_H