#include "Multiplayer/multiboard.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

MultiBoard::MultiBoard(bool isHost, QWebSocketServer* server, QList<QWebSocket*> clients,
                     QWebSocket* clientSocket, const QHash<QString, QString>& playerRoles,
                     const QString& currentUsername, QWidget* parent)
    : QWidget(parent), 
      m_isHost(isHost), 
      m_server(server), 
      m_clients(clients),
      m_clientSocket(clientSocket), 
      m_playerRoles(playerRoles), 
      m_currentUsername(currentUsername), 
      m_currentTurnIndex(0),
      m_turnOrder{"red_spymaster", "red_operative", "blue_spymaster", "blue_operative"} 
{
    // Ensure server and client pointers are valid
    if (!m_server && !m_clientSocket) {
        qWarning() << "Invalid network configuration";
        return;
    }
    
    // Setup network connections
    if (m_isHost) {
        if (m_server) {
            connect(m_server, &QWebSocketServer::newConnection, this, &MultiBoard::handleNewConnection);
        }
    } else {
        if (m_clientSocket) {
            connect(m_clientSocket, &QWebSocket::textMessageReceived, this, &MultiBoard::processMessage);
            connect(m_clientSocket, &QWebSocket::disconnected, this, &MultiBoard::socketDisconnected);
        }
    }
    
    // Setup UI
    setupUI();

    // Get current player's role
    m_currentRole = m_playerRoles.value(m_currentUsername, "Unassigned");
    m_playerInfoLabel->setText(QString("%1 (%2)").arg(m_currentUsername).arg(m_currentRole));

    // Initialize word list with randomization
    initializeWords();

    if (m_isHost) {
        initializeBoardColors();
        
        // Send initial game state
        sendInitialGameState();
    }
   
    setupBoard();
    updateTurnDisplay();
}
void MultiBoard::handleNewConnection()
{
    if (!m_server) return;

    QWebSocket* client = m_server->nextPendingConnection();
    if (!client) return;

    connect(client, &QWebSocket::textMessageReceived, this, &MultiBoard::processMessage);
    m_clients.append(client);
}

void MultiBoard::initializeWords()
{
    // Use QRandomGenerator for better randomization
    QRandomGenerator* generator = QRandomGenerator::global();
    
    m_words = QStringList{"Apple", "Banana", "Car", "Dog", "Egg",
               "Fish", "Gate", "Hat", "Ice", "Jug",
               "Kite", "Lion", "Moon", "Nest", "Oven",
               "Pig", "Queen", "Rose", "Sun", "Tree",
               "Umbrella", "Van", "Watch", "Xylophone", "Yacht"};
    
    // Shuffle the words
    std::shuffle(m_words.begin(), m_words.end(), *generator);
}

void MultiBoard::initializeBoardColors()
{
    QRandomGenerator* generator = QRandomGenerator::global();
    
    // Shuffle colors to randomize board layout
    m_tileColors = QStringList{"red", "red", "red", "red", "red",
                    "red", "red", "red", "red", "blue", 
                    "blue", "blue", "blue", "blue", 
                    "blue", "blue", "neutral", "neutral", 
                    "neutral", "neutral", "neutral", 
                    "neutral", "neutral", "black"};
    
    std::shuffle(m_tileColors.begin(), m_tileColors.end(), *generator);
}

void MultiBoard::sendInitialGameState()
{
    // Validate game state before sending
    if (m_words.size() != 25 || m_tileColors.size() != 25) {
        qWarning() << "Invalid game state";
        return;
    }

    sendToAll("BOARD_SETUP:" + m_words.join(",") + "|" + m_tileColors.join(","));
    sendToAll("TURN_UPDATE:" + m_turnOrder.first());
}

void MultiBoard::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Player info and turn display
    QHBoxLayout* infoLayout = new QHBoxLayout();
    m_playerInfoLabel = new QLabel(this);
    m_turnLabel = new QLabel(this);
    infoLayout->addWidget(m_playerInfoLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(m_turnLabel);
    mainLayout->addLayout(infoLayout);

    m_grid = new QGridLayout();
    mainLayout->addLayout(m_grid);
}
void MultiBoard::setupBoard()
{
    // Clean up existing tiles
    qDeleteAll(m_tiles);
    m_tiles.clear();
    
    // Create new board tiles
    for(int i = 0; i < 25; i++) {
        QPushButton* btn = new QPushButton(m_words[i]);
        btn->setMinimumSize(100, 60);
        btn->setStyleSheet("QPushButton { background: #FFFFFF; font-size: 14px; }");
        connect(btn, &QPushButton::clicked, this, &MultiBoard::handleTileClick);
        m_grid->addWidget(btn, i/5, i%5);
        m_tiles.append(btn);
        
        // IMPORTANT: Always enable tiles initially
        btn->setEnabled(true);
    }
}

void MultiBoard::processMessage(const QString& message)
{
    qDebug() << "Received message:" << message;
    
     qDebug() << "Received message:" << message;
    
    if(message.startsWith("BOARD_SETUP:")) {
        QString data = message.section(':', 1);
        QStringList parts = data.split("|");
        
        // Validate message format
        if(parts.size() != 2 || 
           parts[0].split(",").size() != 25 || 
           parts[1].split(",").size() != 25) {
            qWarning() << "Invalid BOARD_SETUP message received";
            return;
        }
        
        m_words = parts[0].split(",");
        m_tileColors = parts[1].split(",");
        setupBoard();
    }
    else if(message.startsWith("REVEAL:")) {
        int index = message.section(':', 1).toInt();
        revealTile(index, false);
    }
    else if(message.startsWith("START_GAME:")) {
        QString data = message.section(':', 1);
        QStringList entries = data.split("|");
        QHash<QString, QString> playerRoles;
        foreach (const QString& entry, entries) {
            QStringList parts = entry.split(":");
            if (parts.size() == 2) {
                playerRoles[parts[0]] = parts[1];
            }
        }
        
        // Update player roles without triggering turn checks
        m_playerRoles = playerRoles;
        m_currentRole = playerRoles.value(m_currentUsername, "Unassigned");
        m_playerInfoLabel->setText(QString("%1 (%2)").arg(m_currentUsername).arg(m_currentRole));
        
        return;
    }
    else if(message.startsWith("TURN_UPDATE:")) {
        QString newTurn = message.section(':', 1);
        m_currentTurnIndex = m_turnOrder.indexOf(newTurn);
        if(m_currentTurnIndex == -1) m_currentTurnIndex = 0;
        updateTurnDisplay();
    }
}

bool MultiBoard::isMyTurn() const
{
    // More verbose debugging
    qDebug() << "Checking Turn:";
    qDebug() << "Current Turn Index:" << m_currentTurnIndex;
    qDebug() << "Turn Order:" << m_turnOrder;
    qDebug() << "My Current Role:" << m_currentRole;
    
    // Ensure we have a valid turn
    if (m_turnOrder.isEmpty() || m_currentTurnIndex < 0 || 
        m_currentTurnIndex >= m_turnOrder.size()) {
        qDebug() << "Invalid turn state";
        return false;
    }
    
    // Get current turn details
    QString currentPhase = m_turnOrder[m_currentTurnIndex];
    QStringList parts = currentPhase.split("_");
    
    // Validate turn phase
    if (parts.size() != 2) {
        qDebug() << "Invalid turn phase:" << currentPhase;
        return false;
    }
    
    QString currentTeam = parts[0];
    QString currentRole = parts[1];
    
    // Determine player's team and role
    QString myTeam = getMyTeam();
    QString myRole = m_currentRole.toLower();
    
    // Detailed debugging
    qDebug() << "Current Team:" << currentTeam;
    qDebug() << "Current Role:" << currentRole;
    qDebug() << "My Team:" << myTeam;
    qDebug() << "My Role:" << myRole;
    
    // More flexible role matching
    bool isTeamMatch = (myTeam == currentTeam);
    bool isRoleMatch = myRole.contains(currentRole);
    
    qDebug() << "Team Match:" << isTeamMatch;
    qDebug() << "Role Match:" << isRoleMatch;
    
    return isTeamMatch && isRoleMatch;
}

void MultiBoard::updateTurnDisplay()
{
    // Get current turn details
    QString currentPhase = m_turnOrder[m_currentTurnIndex];
    QStringList parts = currentPhase.split("_");
    QString team = parts[0];
    QString role = parts[1];
    
    // Update turn label
    m_turnLabel->setText(QString("Current Turn: %1 %2")
                        .arg(team.toUpper())
                        .arg(role.toUpper()));
    
    // Debug logging
    qDebug() << "Current Phase:" << currentPhase;
    qDebug() << "Is My Turn:" << isMyTurn();
    qDebug() << "My Role:" << m_currentRole;
    
    // More lenient tile enabling
    bool enableTiles = true; // Default to enabled
    if (role == "operative") {
        enableTiles = isMyTurn();
    }
    
    foreach(QPushButton* btn, m_tiles) {
        // Only disable if already revealed
        btn->setEnabled(enableTiles && btn->text() != "");
    }
}

void MultiBoard::handleTileClick()
{
    // Detailed turn checking
    if(!isMyTurn()) {
        QMessageBox::information(this, "Not Your Turn", 
                                 "It's not your turn to act!");
        return;
    }
    
    // Verify current phase allows tile revealing
    QString currentPhase = m_turnOrder[m_currentTurnIndex];
    QStringList parts = currentPhase.split("_");
    if(parts.size() != 2 || parts[1] != "operative") {
        QMessageBox::information(this, "Invalid Action", 
                                 "Only operatives can reveal tiles!");
        return;
    }
    
    // Find the clicked tile
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int index = m_tiles.indexOf(btn);

    // Ensure the tile hasn't been already revealed
    if(btn->text().isEmpty()) {
        QMessageBox::information(this, "Invalid Action", 
                                 "This tile has already been revealed!");
        return;
    }

    if(m_isHost) {
        // Host directly reveals tile and advances turn
        revealTile(index, true);
        advanceTurn();
    } else {
        // Client sends reveal request to host
        m_clientSocket->sendTextMessage(QString("REVEAL:%1").arg(index));
    }
}

void MultiBoard::revealTile(int index, bool broadcast)
{
    // Validate index
    if(index < 0 || index >= 25) return;
    
    // Get tile color and apply styling
    QString color = m_tileColors[index];
    m_tiles[index]->setStyleSheet(getColorStyle(color));
    
    // Clear the text to mark as revealed
    m_tiles[index]->setText("");
    m_tiles[index]->setEnabled(false);

    // Broadcast reveal to other players if needed
    if(broadcast) {
        sendToAll(QString("REVEAL:%1").arg(index));
    }
}

void MultiBoard::advanceTurn()
{
    // Only host can advance turn
    
    // Cycle to next turn
    m_currentTurnIndex = (m_currentTurnIndex + 1) % 4;
    QString nextTurn = m_turnOrder[m_currentTurnIndex];
    
    // Send turn update to all players
    sendToAll(QString("TURN_UPDATE:%1").arg(nextTurn));
    updateTurnDisplay();
}





QString MultiBoard::getMyTeam() const
{
    // Determine player's team based on role
    if(m_currentRole.startsWith("RED", Qt::CaseInsensitive)) return "red";
    if(m_currentRole.startsWith("BLUE", Qt::CaseInsensitive)) return "blue";
    return "";
}

QString MultiBoard::getColorStyle(const QString& color) const
{
    // Return CSS styling for different tile colors
    if(color == "red") return "background: #FF4444; color: white;";
    if(color == "blue") return "background: #4444FF; color: white;";
    if(color == "black") return "background: #000000; color: white;";
    return "background: #CCCCCC; color: black;";
}

void MultiBoard::sendToAll(const QString& message)
{
    // Send message to all connected clients
    for(QWebSocket* client : m_clients) {
        if(client && client->isValid()) {
            client->sendTextMessage(message);
        }
    }
    
    // Log message if host
    if(m_isHost) qDebug() << "Host sent:" << message;
}

void MultiBoard::socketDisconnected()
{
    // Handle client socket disconnection
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    if(client) {
        m_clients.removeAll(client);
        client->deleteLater();
    }
}