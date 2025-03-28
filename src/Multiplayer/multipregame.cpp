#include "Multiplayer/multipregame.h"
#include "Multiplayer/multiboard.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QLoggingCategory>

MultiPregame::MultiPregame(QWebSocketServer* server, const QString& username, QWidget* parent)
    : QWidget(parent), m_server(server), m_isHost(true) {
    m_usernames[nullptr] = username; // Host has null socket
    m_roles[nullptr] = "Unassigned"; // Initialize host's role
    setupUI();
    connect(m_server, &QWebSocketServer::newConnection, this, &MultiPregame::onNewConnection);
}

MultiPregame::MultiPregame(QWebSocket* socket, const QString& username, QWidget* parent)
    : QWidget(parent), m_clientSocket(socket), m_username(username), m_isHost(false) {
    setupUI();
    connect(m_clientSocket, &QWebSocket::textMessageReceived, this, &MultiPregame::processMessage);
    connect(m_clientSocket, &QWebSocket::disconnected, this, &MultiPregame::socketDisconnected);
    // Send username to host immediately after connecting
    m_clientSocket->sendTextMessage("USERNAME:" + username);
}

void MultiPregame::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Player list
    playerList = new QListWidget(this);
    playerList->setStyleSheet("background: rgba(255, 255, 255, 150); border-radius: 10px;");
    layout->addWidget(playerList);

    // Role selection buttons
    QHBoxLayout* roleLayout = new QHBoxLayout();
    QPushButton* redSpymaster = new QPushButton("Red Spymaster", this);
    QPushButton* blueSpymaster = new QPushButton("Blue Spymaster", this);
    QPushButton* redOperative = new QPushButton("Red Operative", this);
    QPushButton* blueOperative = new QPushButton("Blue Operative", this);

    auto styleButton = [](QPushButton* btn, const QString& color) {
        btn->setStyleSheet(QString("background: %1; color: white; border-radius: 5px;").arg(color));
    };

    styleButton(redSpymaster, "#ff4444");
    styleButton(blueSpymaster, "#4444ff");
    styleButton(redOperative, "#ff8888");
    styleButton(blueOperative, "#8888ff");

    if (m_isHost) {
        connect(redSpymaster, &QPushButton::clicked, [this]() { handleRoleSelection("ROLE:RED_SPYMASTER", nullptr); });
        connect(blueSpymaster, &QPushButton::clicked, [this]() { handleRoleSelection("ROLE:BLUE_SPYMASTER", nullptr); });
        connect(redOperative, &QPushButton::clicked, [this]() { handleRoleSelection("ROLE:RED_OPERATIVE", nullptr); });
        connect(blueOperative, &QPushButton::clicked, [this]() { handleRoleSelection("ROLE:BLUE_OPERATIVE", nullptr); });
    } else {
        connect(redSpymaster, &QPushButton::clicked, [this]() { m_clientSocket->sendTextMessage("ROLE:RED_SPYMASTER"); });
        connect(blueSpymaster, &QPushButton::clicked, [this]() { m_clientSocket->sendTextMessage("ROLE:BLUE_SPYMASTER"); });
        connect(redOperative, &QPushButton::clicked, [this]() { m_clientSocket->sendTextMessage("ROLE:RED_OPERATIVE"); });
        connect(blueOperative, &QPushButton::clicked, [this]() { m_clientSocket->sendTextMessage("ROLE:BLUE_OPERATIVE"); });
    }

    roleLayout->addWidget(redSpymaster);
    roleLayout->addWidget(blueSpymaster);
    roleLayout->addWidget(redOperative);
    roleLayout->addWidget(blueOperative);
    layout->addLayout(roleLayout);

    // Start game button (host only)
    if (m_isHost) {
        QPushButton* startButton = new QPushButton("Start Game", this);
        startButton->setStyleSheet("background: #4CAF50; color: white; padding: 10px;");
        connect(startButton, &QPushButton::clicked, this, &MultiPregame::startGame);
        layout->addWidget(startButton);
    }

    // IP Address Display
    QString ipAddress;
    if (m_isHost) {
        const QList<QHostAddress> ipAddresses = QNetworkInterface::allAddresses();
        for (const QHostAddress &address : ipAddresses) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost)
                ipAddress = address.toString();
        }
        QLabel* ipLabel = new QLabel(QString("Lobby IP: %1:%2").arg(ipAddress).arg(m_server->serverPort()), this);
        layout->addWidget(ipLabel);
    } else {
        QLabel* ipLabel = new QLabel(QString("Connected to: %1:%2")
                                     .arg(m_clientSocket->peerAddress().toString())
                                     .arg(m_clientSocket->peerPort()), this);
        layout->addWidget(ipLabel);
    }

    // Back button
    QPushButton* backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, [this]() {
        emit backToMultiMain();
    });
    layout->addWidget(backButton);

    sendLobbyUpdate();
}

void MultiPregame::onNewConnection() {
    qDebug() << "New connection";
    if(m_clients.size() == 3) {
        QMessageBox::warning(this, "Too many players", "The maximum number of players has been reached.");
        return;
    }
    QWebSocket* client = m_server->nextPendingConnection();
    connect(client, &QWebSocket::textMessageReceived, this, &MultiPregame::processMessage);
    connect(client, &QWebSocket::disconnected, this, &MultiPregame::socketDisconnected);
    m_clients.append(client);
    m_roles[client] = "Unassigned"; // Initialize client role
    sendLobbyUpdate();
}

void MultiPregame::processMessage(const QString& message) {
    if (message.startsWith("USERNAME:")) {
        QString username = message.section(':', 1);
        QWebSocket* sender = qobject_cast<QWebSocket*>(this->sender());
        m_usernames[sender] = username;
        if (m_isHost) sendLobbyUpdate();
    } else if (message.startsWith("ROLE:")) {
        handleRoleSelection(message, qobject_cast<QWebSocket*>(this->sender()));
    } else if (message.startsWith("LOBBY_UPDATE:")) {
        QString playersData = message.section(':', 1);
        QStringList players = playersData.split("|");
        playerList->clear();
        playerList->addItems(players);
    } else if(message.startsWith("ROLE_TAKEN:")) {
        QString role = message.section(':', 1);
        QMessageBox::warning(this, "Role Taken", QString("The role %1 has already been taken.").arg(role));
    } else if(message.startsWith("START_GAME:")) {
        QString data = message.section(':', 1);
        QStringList entries = data.split("|");
        QHash<QString, QString> playerRoles;
        foreach (const QString& entry, entries) {
            QStringList parts = entry.split(":");
            if (parts.size() == 2) {
                playerRoles[parts[0]] = parts[1];
            }
        }
        emit gameStarted(false, nullptr, QList<QWebSocket*>(), m_clientSocket, playerRoles);
    }
}

void MultiPregame::sendLobbyUpdate() {
    QStringList players;
    players << QString("%1 (%2)").arg(m_usernames[nullptr]).arg(m_roles.value(nullptr, "Unassigned"));

    for (QWebSocket* client : m_clients) {
        players << QString("%1 (%2)").arg(m_usernames[client]).arg(m_roles.value(client, "Unassigned"));
    }

    playerList->clear();
    playerList->addItems(players);

    if (m_isHost) {
        QString message = "LOBBY_UPDATE:" + players.join("|");
        for (QWebSocket* client : m_clients) {
            client->sendTextMessage(message);
        }
    }
}

void MultiPregame::socketDisconnected() {
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    m_clients.removeAll(client);
    m_usernames.remove(client);
    m_roles.remove(client);
    sendLobbyUpdate();
    client->deleteLater();
}

void MultiPregame::startGame() {
   /* if(m_clients.size() != 3) {
        QMessageBox::warning(this, "Wrong Amount of Players", "You need 4 players to start the game.");
        return;
    }
    if(m_roles[nullptr] == "Unassigned") {
        QMessageBox::warning(this, "Role Not Selected", "You need to select a role for the host.");
        return;
    }
    for (QWebSocket* client : m_clients) {
        if (m_roles[client] == "Unassigned") {
            QMessageBox::warning(this, "Role Not Selected", "You need to select a role for each player.");
            return;
        }
    }*/
    
    QHash<QString, QString> playerRoles;
    playerRoles[m_usernames[nullptr]] = m_roles[nullptr];
    for (QWebSocket* client : m_clients) {
        playerRoles[m_usernames[client]] = m_roles[client];
    }

    QStringList playerRoleList;
    for (auto it = playerRoles.begin(); it != playerRoles.end(); ++it) {
        playerRoleList << QString("%1:%2").arg(it.key()).arg(it.value());
    }
    const QString message = "START_GAME:" + playerRoleList.join("|");
    for (QWebSocket* client : m_clients) {
        client->sendTextMessage(message);
    }

    gameStarted(true, m_server, m_clients, nullptr, playerRoles);
}

void MultiPregame::gameStarted(bool isHost, QWebSocketServer* server, 
                              const QList<QWebSocket*>& clients, QWebSocket* clientSocket,
                              const QHash<QString, QString>& playerRoles) {
    QString currentUsername = m_isHost ? m_usernames[nullptr] : m_username;
    MultiBoard* gameBoard = new MultiBoard(isHost, server, clients, clientSocket, playerRoles, currentUsername);
    
    // Transfer ownership to MultiBoard
    if (isHost) {
        if (server) {
            server->setParent(gameBoard);
            foreach (QWebSocket* client, clients) {
                client->setParent(gameBoard);
                disconnect(client, &QWebSocket::textMessageReceived, this, &MultiPregame::processMessage);
                connect(client, &QWebSocket::textMessageReceived, gameBoard, &MultiBoard::processMessage);
            }
        }
    } else {
        if (clientSocket) {
            clientSocket->setParent(gameBoard);
           
        }
    }
    
    gameBoard->show();
    this->close();
}

void MultiPregame::handleRoleSelection(const QString& message, QWebSocket* sender) {
    QString role = message.section(':', 1);
    if (m_roles[sender] == role) {
        role = "Unassigned";
         m_roles[sender] = role;
        sendLobbyUpdate();
    }

    bool roleTaken = (m_roles[nullptr] == role);
    if (!roleTaken) {
        for (QWebSocket* client : m_clients) {
            if (m_roles[client] == role) {
                roleTaken = true;
                break;
            }
        }
    }

    if (roleTaken) {
        if (sender) {
            sender->sendTextMessage("ROLE_TAKEN:" + role);
        } else {
            QMessageBox::warning(this, "Role Taken", QString("The role %1 is already taken.").arg(role));
        }
        return;
    }

    m_roles[sender] = role;
    sendLobbyUpdate();
}

MultiPregame::~MultiPregame() {
    // Ownership transferred to MultiBoard, no deletion needed here
}