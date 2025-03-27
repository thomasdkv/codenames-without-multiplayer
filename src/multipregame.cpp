#include "multipregame.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkInterface>

MultiPregame::MultiPregame(QWebSocketServer* server, const QString& username, QWidget* parent)
: QWidget(parent), m_server(server), m_isHost(true) {
    m_usernames[nullptr] = username; // Host has null socket
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

    // Role selection buttons (for all players, including host)
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
    QWebSocket* client = m_server->nextPendingConnection();
    connect(client, &QWebSocket::textMessageReceived, this, &MultiPregame::processMessage);
    connect(client, &QWebSocket::disconnected, this, &MultiPregame::socketDisconnected);
    m_clients.append(client);
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
    } else if(message.startsWith("START_GAME:")) {
        startGame();
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
    //1 host 3 clients
    if(m_clients.size() != 3) {
        QMessageBox::warning(this, "Wrong Amount of Players", "You need 4 players to start the game." + QString::number(m_clients.size()));
        return;
    }
    if(m_roles[nullptr] == "Unassigned" || m_roles[nullptr] == "") {
        QMessageBox::warning(this, "Role Not Selected", "You need to select a role for the host.");
        return;
    }
    for (QWebSocket* client : m_clients) {
        if (m_roles[client] == "Unassigned" || m_roles[client] == "") {
            QMessageBox::warning(this, "Role Not Selected", "You need to select a role for each player.");
            return;
        }
        }
        

    for (QWebSocket* client : m_clients) {
        client->sendTextMessage("START_GAME");
    }
    QMessageBox::information(this, "Game Starting", "Starting the game!");
}

void MultiPregame::handleRoleSelection(const QString& message, QWebSocket* sender) {
    QString role = message.section(':', 1);
    //Making sure the role isnt taken already
    if(m_roles[nullptr] == role) {
        QMessageBox::warning(this, "Role Taken", "The role " + role + " is already taken.");
        return;
    }
    for (QWebSocket* client : m_clients) {
        qDebug() << m_roles[client] << role;
        if (m_roles[client] == role ) {
            QMessageBox::warning(this, "Role Taken", "The role " + role + " is already taken.");
            return;
        }
    }
    m_roles[sender] = role;
    sendLobbyUpdate();
}

MultiPregame::~MultiPregame() {
    if (m_server) m_server->deleteLater();
    if (m_clientSocket) m_clientSocket->deleteLater();
}
