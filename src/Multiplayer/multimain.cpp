#include "Multiplayer/multimain.h"
#include "Multiplayer/multipregame.h"
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QPalette>
#include <QInputDialog>
#include <QNetworkInterface>
#include <QMessageBox>

MultiMain::MultiMain(QWidget *parent)
    : QWidget(parent)
{
    // Set background style
    this->setStyleSheet(
        "background-image: url(:/images/menu-background.png);"
        "background-position: center;"
    );

    // Layout for widgets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // Title label
    titleLabel = new QLabel("C++Names", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 50px;");

    // Lobby list
    lobbyList = new QListWidget(this);
    lobbyList->setFixedSize(300, 200);
    lobbyList->setStyleSheet(
        "background: rgba(255, 255, 255, 150);"
        "font-size: 16px;"
        "border-radius: 10px;"
        "padding: 5px;"
    );

    // Button styling
    QString buttonStyles = "QPushButton {"
        "   background-color: rgb(65, 42, 213);"
        "   color: white;"
        "   border-radius: 5px;"
        "   border: 2px solid #412AD5;"
        "   padding: 5px;"
        "   font-weight: bold;"
        "   font-size: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(54, 35, 177);"
        "}";

    // Function to create styled buttons with drop shadows
    auto createButton = [this, &buttonStyles](const QString &text) {
        QPushButton *button = new QPushButton(text, this);
        button->setFixedSize(200, 50);
        button->setStyleSheet(buttonStyles);
        
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(5);
        shadow->setOffset(0, 3);
        shadow->setColor(Qt::black);
        button->setGraphicsEffect(shadow);
        
        return button;
    };

    createRoomButton = createButton("Create Room");
    joinRoomButton = createButton("Join Room");
    backButton = createButton("Back");

    // Add widgets to layout
    layout->addWidget(titleLabel);
    layout->addWidget(createRoomButton);
    layout->addWidget(joinRoomButton);
    layout->addWidget(lobbyList);
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, &MultiMain::openMainWindow);
    connect(createRoomButton, &QPushButton::clicked, this, &MultiMain::onCreateRoomClicked);
    connect(joinRoomButton, &QPushButton::clicked, this, &MultiMain::onJoinRoomClicked);

    connect(this, &MultiMain::enterPregameAsHost, [this](QWebSocketServer* s, const QString& u) {
    MultiPregame* pregame = new MultiPregame(s, u);
    pregame->show();
    this->hide();
    
    connect(pregame, &MultiPregame::backToMultiMain, [this, pregame]() {
        pregame->close();
        pregame->deleteLater();
        this->show();
    });
});


    connect(this, &MultiMain::enterPregameAsClient, [this](QWebSocket* s, const QString& u) {
        MultiPregame* pregame = new MultiPregame(s, u);
        pregame->show();
        this->hide();
        
        connect(pregame, &MultiPregame::backToMultiMain, [this, pregame]() {
            pregame->close();
            pregame->deleteLater();
            this->show();
        });
    });

}

MultiMain::~MultiMain()
{
    if(m_server) m_server->deleteLater();
    if(m_clientSocket) m_clientSocket->deleteLater();
}

void MultiMain::openMainWindow() {
    this->hide();
    emit backToMainWindow();
}

void MultiMain::showMainWindow() {
    this->show();
}

void MultiMain::onCreateRoomClicked() {
    bool ok;
    QString username = QInputDialog::getText(this, "Username", 
        "Enter your username:", QLineEdit::Normal, "", &ok);
    if(!ok || username.isEmpty()) return;

    m_server = new QWebSocketServer("Game Server", QWebSocketServer::NonSecureMode, this);
    
    if (m_server->listen(QHostAddress::AnyIPv4, 12345)) {
        emit enterPregameAsHost(m_server, username);
        this->hide();
    }
}

void MultiMain::onNewConnection() {
    QWebSocket* client = m_server->nextPendingConnection();
    
    connect(client, &QWebSocket::textMessageReceived, 
        this, [this, client](const QString &message) {
            if(message.startsWith("USERNAME:")) {
                QString username = message.mid(9);
                m_usernames[client] = username;
                updateLobbyList();
                sendLobbyListToAll();
            }
            else {
                processTextMessage(message);
            }
        });
    
    connect(client, &QWebSocket::disconnected, this, &MultiMain::socketDisconnected);
    m_clients.append(client);
}

void MultiMain::socketDisconnected() {
    if(QWebSocket* client = qobject_cast<QWebSocket*>(sender())) {
        m_clients.removeAll(client);
        m_usernames.remove(client);
        updateLobbyList();
        sendLobbyListToAll();
        client->deleteLater();
    }
}

void MultiMain::updateLobbyList() {
    lobbyList->clear();
    lobbyList->addItem("Host: You");
    for(const QString& user : m_usernames.values()) {
        lobbyList->addItem("Player: " + user);
    }
}

void MultiMain::sendLobbyListToAll() {
    QStringList users;
    for(const QString& user : m_usernames.values()) {
        users << user;
    }
    QString message = "LOBBY_UPDATE:" + users.join(",");
    for(QWebSocket* client : m_clients) {
        client->sendTextMessage(message);
    }
}

void MultiMain::onJoinRoomClicked() {
    bool ok;
    QString host = QInputDialog::getText(this, "Join Room", 
                                       "Server IP:", QLineEdit::Normal, 
                                       "localhost", &ok);
    if(!ok || host.isEmpty()) return;

    int port = QInputDialog::getInt(this, "Join Room", 
                                   "Port:", 12345, 1, 65535, 1, &ok);
    if(!ok) return;

    QString username = QInputDialog::getText(this, "Username", 
        "Enter your username:", QLineEdit::Normal, "", &ok);
    if(!ok || username.isEmpty()) return;

    m_username = username;
    m_clientSocket = new QWebSocket;
    
    connect(m_clientSocket, &QWebSocket::connected, this, [this]() {
        m_clientSocket->sendTextMessage("USERNAME:" + m_username);
        joinRoomButton->setEnabled(false);
        titleLabel->setText("Connected to room!");
    });
    connect(m_clientSocket, &QWebSocket::connected, this, [this, username]() {
    m_clientSocket->sendTextMessage("USERNAME:" + username);
    emit enterPregameAsClient(m_clientSocket, username);
    this->hide();
    });

    connect(m_clientSocket, &QWebSocket::disconnected, this, &MultiMain::onDisconnected);
    
    connect(m_clientSocket, &QWebSocket::textMessageReceived,
        this, [this](const QString& message) {
            if(message.startsWith("LOBBY_UPDATE:")) {
                lobbyList->clear();
                QStringList users = message.mid(13).split(",");
                lobbyList->addItem("Host: ???");
                for(const QString& user : users) {
                    lobbyList->addItem("Player: " + user);
                }
            }
            else {
                QMessageBox::information(this, "Message", message);
            }
        });

    m_clientSocket->open(QUrl(QString("ws://%1:%2").arg(host).arg(port)));
}

void MultiMain::onConnected() {
    // Handled in lambda
}

void MultiMain::onDisconnected() {
    QMessageBox::warning(this, "Disconnected", "Lost connection to room");
    joinRoomButton->setEnabled(true);
    lobbyList->clear();
}

void MultiMain::processTextMessage(QString message) {
    if(QWebSocket* sender = qobject_cast<QWebSocket*>(this->sender())) {
        for(QWebSocket* client : m_clients) {
            if(client != sender) {
                client->sendTextMessage(message);
            }
        }
    }
}