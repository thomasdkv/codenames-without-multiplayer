#include "Multiplayer/multimain.h"
#include "Multiplayer/multipregame.h"
#include "Multiplayer/multiboard.h"
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QPalette>
#include <QInputDialog>
#include <QNetworkInterface>
#include <QMessageBox>

MultiMain::MultiMain(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(1000, 600);
    // Set background style
    this->setStyleSheet(
        "background-image: url(:/images/menu-background.png);"
        "background-position: center;");

    // Layout for widgets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // Title label
    titleLabel = new QLabel("C++Names", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 50px;");

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
    auto createButton = [this, &buttonStyles](const QString &text)
    {
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
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, &MultiMain::openMainWindow);
    connect(createRoomButton, &QPushButton::clicked, this, &MultiMain::onCreateRoomClicked);
    connect(joinRoomButton, &QPushButton::clicked, this, &MultiMain::onJoinRoomClicked);

    connect(this, &MultiMain::enterPregameAsHost, [this](QWebSocketServer *s, const QString &u)
            {
    MultiPregame* pregame = new MultiPregame(s, u);
    pregame->show();
    this->hide();

    
    connect(pregame, &MultiPregame::backToMultiMain, [this, pregame]() {
        pregame->close();
        pregame->deleteLater();
        this->show();
    }); });

    connect(this, &MultiMain::enterPregameAsClient, [this](QWebSocket *s, const QString &u)
            {
        MultiPregame* pregame = new MultiPregame(s, u);
        pregame->show();
        this->hide();
        
        connect(pregame, &MultiPregame::backToMultiMain, [this, pregame]() {
            pregame->close();
            pregame->deleteLater();
            this->show();
        }); });
}

MultiMain::~MultiMain()
{
    if (m_server)
        m_server->deleteLater();
    if (m_clientSocket)
        m_clientSocket->deleteLater();
}

void MultiMain::openMainWindow()
{
    this->hide();
    emit backToMainWindow();
}

void MultiMain::showMainWindow()
{
    this->show();
}

void MultiMain::onCreateRoomClicked()
{
    // Load usernames from the JSON file
    User *user = User::instance();
    QJsonObject jsonData = user->loadJsonFile();

    QStringList usernames;
    QJsonObject::const_iterator it = jsonData.constBegin();
    while (it != jsonData.constEnd())
    {
        usernames.append(it.key()); // Extract username keys
        ++it;
    }

    // If there are no usernames, show a message and return
    if (usernames.isEmpty())
    {
        QMessageBox::warning(this, "No Users", "No user profiles found. Please create an account first.");
        return;
    }

    // Create a dialog for selecting a username
    QDialog dialog(this);
    dialog.setWindowTitle("Select Username");

    QVBoxLayout layout(&dialog);
    QLabel label("Select your username:");
    QComboBox comboBox;
    QHBoxLayout buttonLayout;
    QPushButton okButton("OK");
    QPushButton cancelButton("Cancel");

    // Add widgets to layouts
    layout.addWidget(&label);
    layout.addWidget(&comboBox);
    buttonLayout.addWidget(&okButton);
    buttonLayout.addWidget(&cancelButton);
    layout.addLayout(&buttonLayout);

    // Populate the combo box with usernames
    comboBox.addItems(usernames);

    // Connect buttons to dialog actions
    connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Execute dialog
    if (dialog.exec() == QDialog::Accepted)
    {
        QString username = comboBox.currentText();
        if (username.isEmpty())
            return;

        m_server = new QWebSocketServer("Game Server", QWebSocketServer::NonSecureMode, this);

        if (m_server->listen(QHostAddress::AnyIPv4, 0))
        {                                                  // 0 means any open port
            quint16 assignedPort = m_server->serverPort(); // Get the assigned port
            emit enterPregameAsHost(m_server, username);
            this->hide();
        }
    }
}

void MultiMain::onNewConnection()
{
    QWebSocket *client = m_server->nextPendingConnection();

    connect(client, &QWebSocket::textMessageReceived,
            this, [this, client](const QString &message)
            {
            if(message.startsWith("USERNAME:")) {
                QString username = message.mid(9);
                m_usernames[client] = username;
              
            }
            else {
                processTextMessage(message);
            } });

    connect(client, &QWebSocket::disconnected, this, &MultiMain::socketDisconnected);
    m_clients.append(client);
}

void MultiMain::socketDisconnected()
{
    if (QWebSocket *client = qobject_cast<QWebSocket *>(sender()))
    {
        m_clients.removeAll(client);
        m_usernames.remove(client);

        client->deleteLater();
    }
}

void MultiMain::onJoinRoomClicked()
{
    bool ok;
    QString host = QInputDialog::getText(this, "Join Room",
                                         "Server IP:", QLineEdit::Normal,
                                         "localhost", &ok);
    if (!ok || host.isEmpty())
        return;

    int port = QInputDialog::getInt(this, "Join Room",
                                    "Port:", 1, 1, 65535, 1, &ok);
    if (!ok)
        return;

    // Load usernames from the JSON file
    User *user = User::instance();
    QJsonObject jsonData = user->loadJsonFile();

    QStringList usernames;
    QJsonObject::const_iterator it = jsonData.constBegin();
    while (it != jsonData.constEnd())
    {
        usernames.append(it.key()); // Extract username keys
        ++it;
    }

    // If there are no usernames, show a message and return
    if (usernames.isEmpty())
    {
        QMessageBox::warning(this, "No Users", "No user profiles found. Please create an account first.");
        return;
    }

    // Create a dialog for selecting a username
    QDialog dialog(this);
    dialog.setWindowTitle("Select Username");

    QVBoxLayout layout(&dialog);
    QLabel label("Select your username:");
    QComboBox comboBox;
    QHBoxLayout buttonLayout;
    QPushButton okButton("OK");
    QPushButton cancelButton("Cancel");

    // Add widgets to layouts
    layout.addWidget(&label);
    layout.addWidget(&comboBox);
    buttonLayout.addWidget(&okButton);
    buttonLayout.addWidget(&cancelButton);
    layout.addLayout(&buttonLayout);

    // Populate the combo box with usernames
    comboBox.addItems(usernames);

    // Connect buttons to dialog actions
    connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Execute dialog
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString username = comboBox.currentText();
    if (username.isEmpty())
        return;

    m_username = username;
    m_clientSocket = new QWebSocket;

    connect(m_clientSocket, &QWebSocket::connected, this, [this]()
            {
        m_clientSocket->sendTextMessage("USERNAME:" + m_username);
        joinRoomButton->setEnabled(false); });

    connect(m_clientSocket, &QWebSocket::connected, this, [this, username]()
            {
        m_clientSocket->sendTextMessage("USERNAME:" + username);
        emit enterPregameAsClient(m_clientSocket, username);
        this->hide(); });

    connect(m_clientSocket, &QWebSocket::disconnected, this, &MultiMain::onDisconnected);

    m_clientSocket->open(QUrl(QString("ws://%1:%2").arg(host).arg(port)));
}

void MultiMain::onConnected()
{
    // Handled in lambda
}

void MultiMain::onDisconnected()
{
    if (m_clientSocket)
    {
        m_clientSocket->close();
    }
    QMessageBox::warning(this, "Disconnected", "Lost connection to room");
    backToMainWindow();
    joinRoomButton->setEnabled(true);
}

void MultiMain::processTextMessage(QString message)
{
    if (QWebSocket *sender = qobject_cast<QWebSocket *>(this->sender()))
    {
        for (QWebSocket *client : m_clients)
        {
            if (client != sender)
            {
                client->sendTextMessage(message);
            }
        }
    }
}