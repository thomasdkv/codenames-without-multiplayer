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
    if (m_isHost)
    {
        loadWordsFromFile();
        generateGameGrid();
    }

    redCardsRemaining = 9;
    blueCardsRemaining = 8;

    setupUI();

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
    gameVerticalLayout->addWidget(hint);
    hint->hide();

    guess = new OperatorGuess(this);
    gameVerticalLayout->addWidget(guess);
    guess->hide();

    connect(hint, &SpymasterHint::hintSubmitted, this, &MultiBoard::advanceTurnSpymaster);
    connect(guess, &OperatorGuess::guessSubmitted, this, &MultiBoard::advanceTurn);
    setupBoard();
    updateTurnDisplay();
}

void MultiBoard::displayHint(const QString &hint, int number)
{
    qDebug() << "Received hint:" << hint << "for" << number << "corresponding words";
    // Update the coreesponding number for the hint, if it is 0, display "∞"
    if (number == 0)
    {
        correspondingNumber = "∞";
    }
    else
    {
        correspondingNumber = QString::number(number);
    }
    currentHint->setText("Current hint: " + hint + " (" + correspondingNumber + ")"); // Update the hint
                                                                                      // Add the hint to the chat box
    QString currSpymasterName = (m_currentTurnIndex == 0) ? "Red Spymaster" : "Blue Spymaster";
    QString teamColor = (m_currentTurnIndex == 0) ? "Red" : "Blue";
    QString chatNumber = (number == 0) ? "∞" : QString::number(number);
    QString hintMessage = currSpymasterName + " gives clue " + hint + " " + chatNumber;
    chatBox->addSystemMessage(hintMessage, (m_currentTurnIndex == RED_SPY) ? ChatBox::RED_TEAM : ChatBox::BLUE_TEAM);
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
    QFile file("resources/wordlist-eng.txt");
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
    int redCards = 9;
    int blueCards = 8;
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
    // Clear existing lists
    m_words.clear();
    m_tileColors.clear();
    QVector<int> cardTypes; // New: Store numerical type codes

    // Validate grid initialization
    if (gameGrid[0][0].word.isEmpty())
    {
        qWarning() << "Game grid not initialized";
        return;
    }

    try
    {
        for (int i = 0; i < GRID_SIZE; ++i)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                // Store words
                QString word = gameGrid[i][j].word.simplified();
                m_words.append(word.isEmpty() ? "UNKNOWN" : word);

                // Store numerical type codes (0=RED, 1=BLUE, 2=NEUTRAL, 3=ASSASSIN)
                cardTypes.append(static_cast<int>(gameGrid[i][j].type));
            }
        }

        // Convert numerical types to string
        QString typeMessage = "";
        for (int type : cardTypes)
        {
            typeMessage += QString::number(type) + ",";
        }
        typeMessage.chop(1); // Remove trailing comma

        // Create full message
        QString fullMessage = "BOARD_SETUP:" +
                              m_words.join(",") + "|" +
                              typeMessage;

        sendToAll(fullMessage);
        sendToAll("TURN_UPDATE:" + m_turnOrder.first());
    }
    catch (const std::exception &e)
    {
        qCritical() << "Error sending game state:" << e.what();
    }
}

void MultiBoard::setupUI()
{
    setFixedSize(1000, 600);
    // Apply main window styles
    setStyleSheet(" font-family: 'Segoe UI', Arial, sans-serif;");
    
    // Main horizontal layout that will hold grid area and chat box
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(25);   // Increased space between grid and chat
    
    // Create vertical layout for the game elements (left side)
    gameVerticalLayout = new QVBoxLayout();
    gameVerticalLayout->setSpacing(15);
    
    // Player info and turn display with improved styling
    QHBoxLayout *infoLayout = new QHBoxLayout();
    m_playerInfoLabel = new QLabel(this);
    m_playerInfoLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #fff; padding: 5px;");
    m_playerInfoLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_playerInfoLabel->setWordWrap(true);
    
    m_turnLabel = new QLabel(this);
    m_turnLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #fff; padding: 5px;");
    m_turnLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_turnLabel->setWordWrap(true);
    
    infoLayout->addWidget(m_playerInfoLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(m_turnLabel);
    gameVerticalLayout->addLayout(infoLayout);
    
    // Add current hint label with enhanced styling and word wrap
    currentHint = new QLabel("Current hint: ");
    currentHint->setAlignment(Qt::AlignCenter);
    currentHint->setStyleSheet("font-weight: bold; font-size: 22px; color: #fff; padding: 10px; border-radius: 8px;");
    currentHint->setWordWrap(true);
    currentHint->setMinimumHeight(50);
    gameVerticalLayout->addWidget(currentHint);
    
   
        // Remaining red and blue cards
    QHBoxLayout *cardsRemainingLayout = new QHBoxLayout();

    redCardText = new QLabel("Red Cards Remaining: " + QString::number(redCardsRemaining));
    redCardText->setStyleSheet("color: red; font-weight: bold; font-size: 16px;");

    blueCardText = new QLabel("Blue Cards Remaining: " + QString::number(blueCardsRemaining));
    blueCardText->setStyleSheet("color: blue; font-weight: bold; font-size: 16px;");

    cardsRemainingLayout->addWidget(redCardText);
    cardsRemainingLayout->addStretch();
    cardsRemainingLayout->addWidget(blueCardText);
    
    gameVerticalLayout->addLayout(cardsRemainingLayout);
    
    // Add grid layout with spacing and styling
    m_grid = new QGridLayout();
    m_grid->setSpacing(10);
    gameVerticalLayout->addLayout(m_grid);
    
    // Add the game vertical layout to the main horizontal layout
    mainLayout->addLayout(gameVerticalLayout);
    
    // Add chat box to the right with styling
    chatBox = new ChatBox(m_currentUsername, ChatBox::RED_TEAM, this);
    chatBox->setMinimumWidth(250); // Ensure chat box has enough width
    connect(chatBox, &ChatBox::massSend, this, &MultiBoard::processChatMessage);
    mainLayout->addWidget(chatBox);
    
    // Set stretch factors
    mainLayout->setStretch(0, 7); // Grid takes more space
    mainLayout->setStretch(1, 3); // Chat box takes reasonable space
    
    setLayout(mainLayout);
}

void MultiBoard::processChatMessage(const QString &playerName, const QString &message)
{
    if (!m_isHost)
    {
        m_clientSocket->sendTextMessage("MESSAGE: " + playerName + " | " + message);
    }
    else
    {
        sendToAll("MESSAGE: " + playerName + " | " + message);
    }
}
void MultiBoard::setupBoard()
{
    // Validate critical components before proceeding
    if (!m_grid)
    {
        qWarning() << "Grid layout is null. Cannot setup board.";
        return;
    }

    // Clean up existing tiles safely
    for (QPushButton *btn : m_tiles)
    {
        m_grid->removeWidget(btn);
        delete btn;
    }
    m_tiles.clear();

    // Additional safety checks
    if (m_words.isEmpty())
    {
        qWarning() << "No words available to setup board";
        return;
    }

    // Ensure we don't exceed available words
    int wordCount = qMin(m_words.size(), GRID_SIZE * GRID_SIZE);

    try
    {
        for (int i = 0; i < GRID_SIZE; ++i)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                int index = i * GRID_SIZE + j;

                // Bounds check to prevent out-of-range access
                if (index >= wordCount)
                {
                    qWarning() << "Attempted to access word beyond available list";
                    break;
                }

                // Safe word retrieval
                QString word = gameGrid[i][j].word; // Use reconstructed word
                if (word.isEmpty())
                {
                    qWarning() << "Empty word at index" << index;
                    continue;
                }

                // Create button with safe memory allocation
                QPushButton *btn = new QPushButton(word);
                if (!btn)
                {
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
                    CardType safeType = (index < GRID_SIZE * GRID_SIZE) ? gameGrid[i][j].type : NEUTRAL;

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
    catch (const std::exception &e)
    {
        qCritical() << "Exception in setupBoard:" << e.what();
    }
}

void MultiBoard::checkGameEnd()
{
    users = User::instance();

    if (redCardsRemaining == 0)
    {
        if (m_currentRole == "red_spymaster" || m_currentRole == "red_operative")
        {
            users->won(m_currentUsername);
        }
        else if (m_currentRole == "blue_spymaster" || m_currentRole == "blue_operative")
        {
            users->lost(m_currentUsername);
        }
        QMessageBox::information(this, "Game Over", "Red team wins!");
    }

    if (blueCardsRemaining == 0)
    {
        if (m_currentRole == "blue_spymaster" || m_currentRole == "blue_operative")
        {
            users->won(m_currentUsername);
        }
        else if (m_currentRole == "red_spymaster" || m_currentRole == "red_operative")
        {
            users->lost(m_currentUsername);
        }
        QMessageBox::information(this, "Game Over", "Blue team wins!");
    }
}

void MultiBoard::processMessage(const QString &message)
{
    qDebug() << "AMAMAMMA";
    if (message.startsWith("TURN_ADVANCE") && m_isHost)
    {
        advanceTurn();
    }
    if (message.startsWith("SPYMASTER_TURN_ADVANCE:") && m_isHost)
    {
        qDebug() << "Received TURN_ADVANCE_SPYMASTER message";
        QString data = message.section(':', 1);
        QStringList parts = data.split(",");
        int number;
        QString hint;
        if (parts.size() == 2)
        {
            hint = parts[0];
            number = parts[1].toInt();
            qDebug() << "Received hint:" << hint << "for" << number << "corresponding words";
            advanceTurnSpymaster(hint, number);
        }
    }

    else if (message.startsWith("BOARD_SETUP:"))
    {
        QString data = message.section(':', 1);
        QStringList parts = data.split("|");

        if (parts.size() != 2 ||
            parts[0].split(",").size() != 25 ||
            parts[1].split(",").size() != 25)
        {
            qWarning() << "Invalid BOARD_SETUP message";
            return;
        }

        // Reconstruct game state
        m_words = parts[0].split(",");
        QStringList typeCodes = parts[1].split(",");

        // Convert type codes to gameGrid
        int index = 0;
        for (int i = 0; i < GRID_SIZE; ++i)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                if (index >= typeCodes.size())
                    break;

                bool ok;
                int type = typeCodes[index].toInt(&ok);
                gameGrid[i][j].type = static_cast<CardType>(ok ? type : NEUTRAL);
                gameGrid[i][j].word = m_words.value(index, "");
                gameGrid[i][j].revealed = false;
                index++;
            }
        }

        setupBoard();
    }
    else if (message.startsWith("REVEAL:"))
    {
        QStringList coords = message.section(':', 1).split(',');
        qDebug() << coords;
        if (coords.size() == 2)
        {
            qDebug() << "Revealing tile at (" << coords[0] << ", " << coords[1] << ")";
            int row = coords[0].toInt();
            int col = coords[1].toInt();
            revealTile(row, col, false);
        }
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
    else if (message.startsWith("UPDATE_HINT:"))
    {
        qDebug() << "Received UPDATE_HINT message: " << message;
        QString data = message.section(':', 1).trimmed();        // Extract data after "UPDATE_HINT:"
        QStringList parts = data.split(",", Qt::SkipEmptyParts); // Ensure no empty elements

        if (parts.size() == 2)
        {
            QString hint = parts[0].trimmed();
            int number = parts[1].trimmed().toInt();

            if (!hint.isEmpty()) // Ensure hint is not empty
            {
                displayHint(hint, number);
            }
            else
            {
                qDebug() << "Hint is empty or only whitespace.";
            }
        }
        else
        {
            qDebug() << "Invalid UPDATE_HINT format: " << message;
        }
    }
    else if (message.startsWith("MESSAGE:"))
    {
        QString sender = message.section(':', 1, 1).section('|', 0, 0).trimmed();
        // Extract message text
        QString messageText = message.section('|', 1).trimmed();
        // Add to chat box
        chatBox->addPlayerMessage(sender, messageText);
    }
    else if (message.startsWith("RED")) {
        redCardsRemaining--;
        redCardText->setText("Red Cards Remaining: " + QString::number(redCardsRemaining));
    }
    else if (message.startsWith("BLUE")) {
        blueCardsRemaining--;
        blueCardText->setText("Blue Cards Remaining: " + QString::number(blueCardsRemaining));
    }
}

bool MultiBoard::isMyTurn() const
{
    // More verbose debugging

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

    isMyTurn();

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

    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    int index = m_tiles.indexOf(btn);
    int row = index / GRID_SIZE;
    int col = index % GRID_SIZE;

    if (gameGrid[row][col].revealed)
        return;

    if (m_isHost)
    {

        revealTile(row, col, true);
        checkGameEnd();
    }
    else
    {
        m_clientSocket->sendTextMessage(QString("REVEAL:%1,%2").arg(row).arg(col));
    }
}

void MultiBoard::revealTile(int row, int col, bool broadcast)
{
    if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
        return;


        QString currOperativeName = (m_currentTurnIndex == 1 || m_currentTurnIndex == 2) ? "Red Operative" : "Blue Operative";
        QString teamColor = (m_currentTurnIndex == 1 || m_currentTurnIndex == 2) ? "Red" : "Blue";
        QString hintMessage = currOperativeName + " taps " + gameGrid[row][col].word;
        chatBox->addSystemMessage(hintMessage, (m_currentTurnIndex == RED_OP || m_currentTurnIndex == BLUE_SPY) ? ChatBox::RED_TEAM : ChatBox::BLUE_TEAM);

    gameGrid[row][col].revealed = true;
    QPushButton *btn = m_tiles.at(row * GRID_SIZE + col);
    btn->setText("");
    btn->setEnabled(false);

    users = User::instance();

    // Get current turn information
    QString currentPhase = m_turnOrder[m_currentTurnIndex];
    QStringList parts = currentPhase.split("_");
    QString currentTeam = parts[0];
    QString currentRole = parts[1];

    if (!m_isHost && broadcast)
    {
        m_clientSocket->sendTextMessage(QString("REVEAL:%1,%2").arg(row).arg(col));
    }
    else
    {
        sendToAll(QString("REVEAL:%1,%2").arg(row).arg(col));
    }
    // Set card color based on type
    switch (gameGrid[row][col].type)
    {
        qDebug() << "Type: " << gameGrid[row][col].type;

    case RED_TEAM:

        btn->setStyleSheet("background-color: #ff9999; color: black");
        if (m_isHost)
        {
            redCardsRemaining--;
            sendToAll(QString("RED"));
            if (redCardsRemaining == 0)
            {
                endGame("Red team wins!");
            }
        }
        break;
    case BLUE_TEAM:

        btn->setStyleSheet("background-color: #9999ff; color: black");
        if (m_isHost)
        {
            blueCardsRemaining--;
            sendToAll(QString("BLUE"));
            if (blueCardsRemaining == 0)
            {
                endGame("Blue team wins!");
            }
        }
        break;
    case NEUTRAL:
        btn->setStyleSheet("background-color: #f0f0f0; color: black");
        break;
    case ASSASSIN:
        btn->setStyleSheet("background-color: #333333; color: white;");
        if (currentPhase == "red_operative" || currentPhase == "blue_spymaster")
        {
            endGame("Blue team wins!");
        }
        else
        {
            endGame("Red team wins!");
        }
        break;
    }

    // Check if correct guess
    bool correctCard = false;
    if (currentTeam == "red" && gameGrid[row][col].type == RED_TEAM)
    {
        if (m_currentRole.toLower() == "red_operative")
        {
            users->hit(m_currentUsername);
        }
        correctCard = true;
    }
    else if (currentTeam == "blue" && gameGrid[row][col].type == BLUE_TEAM)
    {
        if (m_currentRole.toLower() == "blue_operative")
        {
            users->hit(m_currentUsername);
        }
        correctCard = true;
    }

    if (!correctCard)
    {

        if (m_currentRole.toLower() == "red_operative" || m_currentRole.toLower() == "blue_operative")
        {
            users->miss(m_currentUsername);
        }
        if (!m_isHost)
        {
            m_clientSocket->sendTextMessage(QString("TURN_ADVANCE"));
        }
        else
        {
            sendToAll(QString("TURN_ADVANCE"));
        }
        return;
    }
    else if (currentRole == "operative")
    {
        // Only allow another guess if operative guessed correctly
        btn->setEnabled(false);
    }

    if (broadcast)
    {
        sendToAll(QString("REVEAL:%1,%2").arg(row).arg(col));
    }
}

void MultiBoard::endGame(const QString &message)
{
    if (!m_isHost)
    {
        // Client: Send message and transfer ownership back
        m_clientSocket->sendTextMessage("END_GAME:" + message);

        // Disconnect from MultiBoard's handler
        disconnect(m_clientSocket, &QWebSocket::textMessageReceived,
                   this, &MultiBoard::processMessage);

        // Reparent clientSocket to original owner
        m_clientSocket->setParent(qobject_cast<QObject *>(this->parent()));

        // Reconnect to MultiPregame's handler
        if (this->parent())
        {
            connect(m_clientSocket, &QWebSocket::textMessageReceived,
                    qobject_cast<MultiPregame *>(this->parent()),
                    &MultiPregame::processMessage);
        }
        QMessageBox::information(this, "Won", "Game Over! " + message);

        emit goBack();
        this->deleteLater();
    }
    else
    {
        // Host: Notify all clients and transfer ownership back
        sendToAll(QString("END_GAME:%1").arg(message));

        if (m_server)
        {
            m_server->setParent(qobject_cast<QObject *>(this->parent()));

            foreach (QWebSocket *client, m_clients)
            {
                disconnect(client, &QWebSocket::textMessageReceived,
                           this, &MultiBoard::processMessage);
                client->setParent(qobject_cast<QObject *>(this->parent()));

                if (this->parent())
                {
                    qDebug() << "Reconnecting to MultiPregame";
                    connect(client, &QWebSocket::textMessageReceived,
                            qobject_cast<MultiPregame *>(this->parent()),
                            &MultiPregame::processMessage);
                }
            }
        }
        QMessageBox::information(this, "Won", "Game Over! " + message);

        emit goBack();
        this->deleteLater();
    }
}
void MultiBoard::advanceTurnSpymaster(const QString &hint, int number)
{
    qDebug() << "Advancing turn 2";
    if (!m_isHost)
    {
        if (m_clientSocket && m_clientSocket->isValid())
        {
            qDebug() << m_clientSocket->isValid();
            m_clientSocket->sendTextMessage("SPYMASTER_TURN_ADVANCE:" + hint + "," + QString::number(number));
        }
        else
        {
            qWarning() << "Client socket not connected!";
        }
        return;
    }

    sendToAll(QString("UPDATE_HINT:%1,%2").arg(hint).arg(number));
    displayHint(hint, number);

    // Host logic: advance turn and notify all clients
    m_currentTurnIndex = (m_currentTurnIndex + 1) % 4;
    QString nextTurn = m_turnOrder[m_currentTurnIndex];
    sendToAll(QString("TURN_UPDATE:%1").arg(nextTurn));
    updateTurnDisplay(); // Host updates UI immediately
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
            try
            {
                client->sendTextMessage(message);
            }
            catch (...)
            {
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