#include "Multiplayer/multiboard.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

MultiBoard::MultiBoard(bool isHost, QWebSocketServer *server, QList<QWebSocket *> clients,
                       QWebSocket *clientSocket, const QHash<QString, QString> &playerRoles,
                       const QString &currentUsername, QWidget *parent)
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
    if (!m_server && !m_clientSocket)
    {
        qWarning() << "Invalid network configuration";
        return;
    }

    // Setup network connections
    if (m_isHost)
    {
        if (m_server)
        {
            qDebug() << "Listening for connections...";
            connect(m_server, &QWebSocketServer::newConnection, this, &MultiBoard::handleNewConnection);
        }
        else
        {
            qWarning() << "Invalid network configuration";
        }
    }
    else
    {
        if (m_clientSocket)
        {
            connect(m_clientSocket, &QWebSocket::textMessageReceived, this, &MultiBoard::processMessage);
            connect(m_clientSocket, &QWebSocket::disconnected, this, &MultiBoard::socketDisconnected);
        }
    }

    // Setup UI
    loadWordsFromFile();
    qDebug() << "Creating MultiBoard UI";
    generateGameGrid();
    qDebug() << "Generated game grid";
    setupUI();
    qDebug() << "Setup UI";

    // Get current player's role
    m_currentRole = m_playerRoles.value(m_currentUsername, "Unassigned");
    m_playerInfoLabel->setText(QString("%1 (%2)").arg(m_currentUsername).arg(m_currentRole));

    // Initialize word list with randomization
    qDebug() << "Sending Game State";
    if (m_isHost)
    {
        // Send initial game state
        sendInitialGameState();
    }
    qDebug() << "Sent Game State";
    hint = new SpymasterHint(this);
    mainLayout->addWidget(hint);
    hint->hide();

    guess = new OperatorGuess(this);
    mainLayout->addWidget(guess);
    guess->hide();

    connect(hint, &SpymasterHint::hintSubmitted, this, &MultiBoard::advanceTurn);
    connect(guess, &OperatorGuess::guessSubmitted, this, &MultiBoard::advanceTurn);
    qDebug() << "Setting up board";
    setupBoard();
    qDebug() << "Board set up";
    updateTurnDisplay();
}
void MultiBoard::handleNewConnection()
{
    qDebug() << "New connection";
    if (!m_server)
        return;
    qDebug() << "New connection2";
    QWebSocket *client = m_server->nextPendingConnection();
    if (!client)
        return;
    qDebug() << "New connection3";
    connect(client, &QWebSocket::textMessageReceived, this, &MultiBoard::processMessage);
    m_clients.append(client);
}

void MultiBoard::loadWordsFromFile()
{
    QFile file(":/resources/wordlist-eng.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "failed to open wordlist-eng.text file, using fallback";
        // Fallback word list if file not found
        wordList = QStringList() << "apple" << "banana" << "cat" << "dog" << "elephant"
                                 << "fish" << "goat" << "horse" << "iguana" << "jelly"
                                 << "kangaroo" << "lion" << "monkey" << "nest" << "owl"
                                 << "penguin" << "queen" << "rabbit" << "snake" << "tiger"
                                 << "umbrella" << "violin" << "whale" << "xray" << "zebra";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
        {
            wordList.append(line);
        }
    }
    qDebug() << "Loaded" << wordList.size() << "words";
    file.close();
}

void MultiBoard::generateGameGrid()
{
    // Ensure we have enough words
    if (wordList.size() < 25)
    {
        qDebug() << "Not enough words to generate a game grid" << wordList.size();
        return;
    }

    // Shuffle words
    for (int i = wordList.size() - 1; i > 0; --i)
    {
        int j = QRandomGenerator::global()->bounded(i + 1);
        wordList.swapItemsAt(i, j);
    }

    // Assign card types (Codenames rules: 9 for starting team, 8 for other team, 1 assassin, 7 neutral)
    bool redStarts = QRandomGenerator::global()->bounded(2) == 0;
    int redCards = redStarts ? 9 : 8;
    int blueCards = redStarts ? 8 : 9;
    int assassinCards = 1;
    int neutralCards = 7;

    QList<CardType> cardTypes;
    for (int i = 0; i < redCards; ++i)
        cardTypes.append(RED_TEAM);
    for (int i = 0; i < blueCards; ++i)
        cardTypes.append(BLUE_TEAM);
    for (int i = 0; i < neutralCards; ++i)
        cardTypes.append(NEUTRAL);
    for (int i = 0; i < assassinCards; ++i)
        cardTypes.append(ASSASSIN);

    // Shuffle card types
    for (int i = cardTypes.size() - 1; i > 0; --i)
    {
        int j = QRandomGenerator::global()->bounded(i + 1);
        cardTypes.swapItemsAt(i, j);
    }

    // Fill the grid
    int wordIndex = 0;
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            gameGrid[i][j].word = wordList[wordIndex];
            gameGrid[i][j].type = cardTypes[wordIndex];
            gameGrid[i][j].revealed = false;
            ++wordIndex;
        }
    }
}

void MultiBoard::sendInitialGameState()
{
    // Clear existing lists to prevent potential data accumulation
    m_words.clear();
    m_tileColors.clear();
    
    // Defensive check: ensure grid is properly initialized
    if (gameGrid[0][0].word.isEmpty()) {
        qWarning() << "Game grid not properly initialized before sending state";
        return;
    }
    
    try {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                // Bounds and validity checking
                if (i < 0 || i >= GRID_SIZE || j < 0 || j >= GRID_SIZE) {
                    qWarning() << "Invalid grid indices:" << i << j;
                    continue;
                }

                // Sanitize word input (optional: remove any problematic characters)
                QString sanitizedWord = gameGrid[i][j].word.simplified();
                if (sanitizedWord.isEmpty()) {
                    qWarning() << "Empty word found at grid position:" << i << j;
                    sanitizedWord = "UNKNOWN";
                }

                // Add word to list
                m_words.append(sanitizedWord);
                
                // Convert card type to color string
                QString color;
                switch (gameGrid[i][j].type) {
                    case RED_TEAM: color = "red"; break;
                    case BLUE_TEAM: color = "blue"; break;
                    case NEUTRAL: color = "neutral"; break;
                    case ASSASSIN: color = "black"; break;
                    default: 
                        qWarning() << "Unknown card type at" << i << j;
                        color = "neutral";
                }
                m_tileColors.append(color);
            }
        }

        // More robust validation
        if (m_words.size() != GRID_SIZE * GRID_SIZE) {
            qCritical() << "Mismatch in grid size. Expected" 
                        << (GRID_SIZE * GRID_SIZE) 
                        << "words, got" << m_words.size();
            return;
        }

        // Prepare message with additional error checking
        QString wordsMessage = m_words.join(",");
        QString colorsMessage = m_tileColors.join(",");

        // Validate message components
        if (wordsMessage.isEmpty() || colorsMessage.isEmpty()) {
            qWarning() << "Failed to generate valid game state message";
            return;
        }

        // Send to all clients (existing protocol)
        QString boardSetupMessage = "BOARD_SETUP:" + wordsMessage + "|" + colorsMessage;
        sendToAll(boardSetupMessage);

        // Ensure turn order is not empty before sending
        if (!m_turnOrder.isEmpty()) {
            sendToAll("TURN_UPDATE:" + m_turnOrder.first());
        } else {
            qWarning() << "Turn order is empty, cannot send turn update";
        }
    }
    catch (const std::exception& e) {
        qCritical() << "Exception in sendInitialGameState:" << e.what();
    }
}

void MultiBoard::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Player info and turn display
    QHBoxLayout *infoLayout = new QHBoxLayout();
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
    // Validate critical components before proceeding
    if (!m_grid) {
        qWarning() << "Grid layout is null. Cannot setup board.";
        return;
    }

    // Clean up existing tiles safely
    for (QPushButton* btn : m_tiles) {
        m_grid->removeWidget(btn);
        delete btn;
    }
    m_tiles.clear();

    // Additional safety checks
    if (m_words.isEmpty()) {
        qWarning() << "No words available to setup board";
        return;
    }

    // Ensure we don't exceed available words
    int wordCount = qMin(m_words.size(), GRID_SIZE * GRID_SIZE);

    try {
        for (int i = 0; i < GRID_SIZE; ++i)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                int index = i * GRID_SIZE + j;
                
                // Bounds check to prevent out-of-range access
                if (index >= wordCount) {
                    qWarning() << "Attempted to access word beyond available list";
                    break;
                }

                // Safe word retrieval
                QString word = m_words.value(index, "");
                if (word.isEmpty()) {
                    qWarning() << "Empty word at index" << index;
                    continue;
                }

                // Create button with safe memory allocation
                QPushButton *btn = new QPushButton(word);
                if (!btn) {
                    qCritical() << "Failed to allocate button";
                    continue;
                }

                btn->setMinimumSize(100, 60);

                // Spymaster view
                if (m_currentRole.toLower() == "red_spymaster" || 
                    m_currentRole.toLower() == "blue_spymaster")
                {
                    btn->setEnabled(false);
                    
                    // Safe type checking
                    CardType safeType = (index < GRID_SIZE * GRID_SIZE) ? 
                        gameGrid[i][j].type : NEUTRAL;

                    switch (safeType)
                    {
                    case RED_TEAM:
                        btn->setStyleSheet("background-color: #ff9999; color: black");
                        break;
                    case BLUE_TEAM:
                        btn->setStyleSheet("background-color: #9999ff; color: black");
                        break;
                    case NEUTRAL:
                        btn->setStyleSheet("background-color: #f0f0f0; color: black");
                        break;
                    case ASSASSIN:
                        btn->setStyleSheet("background-color: #333333; color: white;");
                        break;
                    default:
                        btn->setStyleSheet("background-color: #cccccc; color: black");
                    }
                }
                else
                {   

                    btn->setStyleSheet("QPushButton { background: #FFFFFF; font-size: 14px; color:black }");
                    btn->setEnabled(true);
                }

                // Connect signal with additional safety
                connect(btn, &QPushButton::clicked, this, &MultiBoard::handleTileClick);
                
                // Add to grid and tiles list
                m_grid->addWidget(btn, i, j);
                m_tiles.append(btn);
            }
        }
    }
    catch (const std::exception& e) {
        qCritical() << "Exception in setupBoard:" << e.what();
    }
}

void MultiBoard::processMessage(const QString &message)
{

    if (message.startsWith("TURN_ADVANCE") && m_isHost)
    {
        qDebug() << "Advancing turn";
        advanceTurn();
    }

    if (message.startsWith("BOARD_SETUP:"))
    {
        QString data = message.section(':', 1);
        QStringList parts = data.split("|");

        // Validate message format
        if (parts.size() != 2 ||
            parts[0].split(",").size() != 25 ||
            parts[1].split(",").size() != 25)
        {
            qWarning() << "Invalid BOARD_SETUP message received";
            return;
        }

        m_words = parts[0].split(",");
        m_tileColors = parts[1].split(",");
        setupBoard();
    }
    else if (message.startsWith("REVEAL:"))
    {
        int index = message.section(':', 1).toInt();
        revealTile(index, false);
    }
    else if (message.startsWith("START_GAME:"))
    {
        QString data = message.section(':', 1);
        QStringList entries = data.split("|");
        QHash<QString, QString> playerRoles;
        foreach (const QString &entry, entries)
        {
            QStringList parts = entry.split(":");
            if (parts.size() == 2)
            {
                playerRoles[parts[0]] = parts[1];
            }
        }

        // Update player roles without triggering turn checks
        m_playerRoles = playerRoles;
        m_currentRole = playerRoles.value(m_currentUsername, "Unassigned");
        m_playerInfoLabel->setText(QString("%1 (%2)").arg(m_currentUsername).arg(m_currentRole));

        return;
    }
    else if (message.startsWith("TURN_UPDATE:"))
    {
        QString newTurn = message.section(':', 1);
        m_currentTurnIndex = m_turnOrder.indexOf(newTurn);
        if (m_currentTurnIndex == -1)
            m_currentTurnIndex = 0;
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
        m_currentTurnIndex >= m_turnOrder.size())
    {
        qDebug() << "Invalid turn state";
        return false;
    }

    // Get current turn details
    QString currentPhase = m_turnOrder[m_currentTurnIndex];
    QStringList parts = currentPhase.split("_");

    // Validate turn phase
    if (parts.size() != 2)
    {
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
    if (isTeamMatch && isRoleMatch)
    {
        if (myRole.toLower() == "red_spymaster" || myRole.toLower() == "blue_spymaster")
        {
            qDebug() << "Spymaster Hint";
            hint->show();
        }
        else if (myRole.toLower() == "red_operative" || myRole.toLower() == "blue_operative")
        {
            qDebug() << "Operative Hint";
            guess->show();
        }
    }
    else
    {
        hint->hide();
        guess->hide();
    }
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
    if (role == "operative")
    {
        enableTiles = isMyTurn();
    }

    foreach (QPushButton *btn, m_tiles)
    {
        // Only disable if already revealed
        btn->setEnabled(enableTiles && btn->text() != "");
    }
}

void MultiBoard::handleTileClick()
{
    // Detailed turn checking
    if (!isMyTurn())
    {
        QMessageBox::information(this, "Not Your Turn",
                                 "It's not your turn to act!");
        return;
    }

    // Verify current phase allows tile revealing
    QString currentPhase = m_turnOrder[m_currentTurnIndex];
    QStringList parts = currentPhase.split("_");
    if (parts.size() != 2 || parts[1] != "operative")
    {
        QMessageBox::information(this, "Invalid Action",
                                 "Only operatives can reveal tiles!");
        return;
    }

    // Find the clicked tile
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    int index = m_tiles.indexOf(btn);

    // Ensure the tile hasn't been already revealed
    if (btn->text().isEmpty())
    {
        QMessageBox::information(this, "Invalid Action",
                                 "This tile has already been revealed!");
        return;
    }

    if (m_isHost)
    {
        // Host directly reveals tile and advances turn
        revealTile(index, true);
        advanceTurn();
    }
    else
    {
        // Client sends reveal request to host
        m_clientSocket->sendTextMessage(QString("REVEAL:%1").arg(index));
    }
}

void MultiBoard::revealTile(int index, bool broadcast)
{
    // Validate index
    if (index < 0 || index >= 25)
        return;

    // Get tile color and apply styling
    QString color = m_tileColors[index];
    m_tiles[index]->setStyleSheet(getColorStyle(color));

    // Clear the text to mark as revealed
    m_tiles[index]->setText("");
    m_tiles[index]->setEnabled(false);

    // Broadcast reveal to other players if needed
    if (broadcast)
    {
        sendToAll(QString("REVEAL:%1").arg(index));
    }
}

void MultiBoard::advanceTurn()
{
    if (!m_isHost)
    {
        if (m_clientSocket && m_clientSocket->isValid())
        {
            qDebug() << "Client sending TURN_ADVANCE";
            qDebug() << m_clientSocket->isValid();
            m_clientSocket->sendTextMessage("TURN_ADVANCE");
        }
        else
        {
            qWarning() << "Client socket not connected!";
        }
        return;
    }

    // Host logic: advance turn and notify all clients
    m_currentTurnIndex = (m_currentTurnIndex + 1) % 4;
    QString nextTurn = m_turnOrder[m_currentTurnIndex];
    sendToAll(QString("TURN_UPDATE:%1").arg(nextTurn));
    updateTurnDisplay(); // Host updates UI immediately
}

QString MultiBoard::getMyTeam() const
{
    // Determine player's team based on role
    if (m_currentRole.startsWith("RED", Qt::CaseInsensitive))
        return "red";
    if (m_currentRole.startsWith("BLUE", Qt::CaseInsensitive))
        return "blue";
    return "";
}

QString MultiBoard::getColorStyle(const QString &color) const
{
    // Return CSS styling for different tile colors
    if (color == "red")
        return "background: #FF4444; color: white;";
    if (color == "blue")
        return "background: #4444FF; color: white;";
    if (color == "black")
        return "background: #000000; color: white;";
    return "background: #CCCCCC; color: black;";
}

void MultiBoard::sendToAll(const QString &message)
{
    // Safely send message to all connected clients
    for (QWebSocket *client : m_clients)
    {
        if (client && client->state() == QAbstractSocket::ConnectedState)
        {
            try {
                client->sendTextMessage(message);
            }
            catch (...) {
                // Silently handle any sending errors
                qDebug() << "Failed to send message to client";
            }
        }
    }

    // Optional: less verbose logging
    if (m_isHost)
        qDebug() << "Message sent";
}

void MultiBoard::socketDisconnected()
{
    // Handle client socket disconnection
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client)
    {
        m_clients.removeAll(client);
        client->deleteLater();
    }
}