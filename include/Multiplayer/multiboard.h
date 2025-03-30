/**
 * @file multiboard.h
 * @brief Header file for the MultiBoard class, which implements a multiplayer game board.
 * @author Group 9
 */

#ifndef MULTIBOARD_H
#define MULTIBOARD_H


#include "../spymasterhint.h"
#include "../operatorguess.h"
#include "Multiplayer/multimain.h"
#include "Multiplayer/multipregame.h"
#include "chatbox.h"

#include "user.h"
#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QFile>

/**
 * @class MultiBoard
 * @brief A widget that implements the multiplayer game board for a team-based word guessing game.
 * 
 * @details The MultiBoard class manages the game state, UI, and network communications
 * for a multiplayer word-guessing game. It handles player turns, card reveals, scoring,
 * and game progression. The game involves two teams (red and blue) with spymasters
 * giving hints and operators making guesses.
 * 
 * @author Group 9
 */
class MultiBoard : public QWidget 
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MultiBoard class.
     * 
     * @details Initializes the game board with network connections and player information.
     * Sets up the UI components and prepares the game state based on whether the player
     * is hosting or joining a game.
     * 
     * @param isHost Boolean indicating if this instance is the host of the game.
     * @param server Pointer to the WebSocket server (if host).
     * @param clients List of connected client WebSockets.
     * @param clientSocket Pointer to this player's WebSocket (if not host).
     * @param playerRoles Hash mapping player names to their roles.
     * @param currentUsername The username of the current player.
     * @param parent Optional parent widget.
     * 
     * @author Group 9
     */
    explicit MultiBoard(
        bool isHost, 
        QWebSocketServer* server, 
        QList<QWebSocket*> clients,
        QWebSocket* clientSocket, 
        const QHash<QString, QString>& playerRoles,
        const QString& currentUsername, 
        QWidget* parent = nullptr//,
      //  MultiPregame* pregame = nullptr
    );
    
    /**
     * @enum CardType
     * @brief Enumeration of possible card types on the game board.
     */
    enum CardType {
        RED_TEAM,   /**< Card belonging to the red team */
        BLUE_TEAM,  /**< Card belonging to the blue team */
        NEUTRAL,    /**< Neutral card not belonging to either team */
        ASSASSIN    /**< Assassin card that ends the game if selected */
    };

    /**
     * @enum Turn
     * @brief Enumeration representing the different turn states in the game.
     */
    enum Turn {
        RED_SPY,    /**< Red team spymaster's turn */
        RED_OP,     /**< Red team operator's turn */
        BLUE_SPY,   /**< Blue team spymaster's turn */
        BLUE_OP     /**< Blue team operator's turn */
    };

    /**
     * @struct Card
     * @brief Structure representing a card on the game board.
     */
    struct Card {
        QString word;   /**< The word displayed on the card */
        CardType type;  /**< The type/team the card belongs to */
        bool revealed;  /**< Whether the card has been revealed */
    };

public slots:
    /**
     * @brief Handles a player clicking on a tile in the game grid.
     * 
     * @details Processes the action when a player clicks on a word tile,
     * revealing the card's team affiliation if it's the player's turn to guess.
     * Updates game state and advances turn if appropriate.
     * 
     * @author Group 9
     */
    void handleTileClick();
    
    /**
     * @brief Processes incoming network messages.
     * 
     * @details Parses and handles various message types from other players,
     * including game state updates, chat messages, and player actions.
     * 
     * @param message The message string received from the network.
     * 
     * @author Group 9
     */
    void processMessage(const QString& message);
    
    /**
     * @brief Handles a disconnection event from the WebSocket.
     * 
     * @details Cleans up resources and updates the game state when a
     * player disconnects from the game.
     * 
     * @author Group 9
     */
    void socketDisconnected();
    
    /**
     * @brief Handles a new client connection to the game server.
     * 
     * @details Accepts new connections and sets up communication channels
     * for new players joining the game. Only used when this instance is the host.
     * 
     * @author Group 9
     */
    void handleNewConnection();

signals:
    /**
     * @brief Signal emitted when returning to the previous screen.
     * 
     * @details Indicates that the player wants to leave the current game
     * and return to the main menu or lobby.
     */
    void goBack();

private:
    // Network setup
    bool m_isHost;                  /**< Whether this instance is the host */
    QWebSocketServer* m_server;     /**< The WebSocket server for hosting */
    QList<QWebSocket*> m_clients;   /**< List of connected client sockets */
    QWebSocket* m_clientSocket;     /**< This player's client socket */
    MultiPregame* m_pregame;        /**< Reference to the pre-game setup screen */

    ChatBox* chatBox;               /**< Chat interface for player communication */

    User* users;                    /**< User information management */

    MultiMain* main;                /**< Main game interface reference */

    // Player information
    QHash<QString, QString> m_playerRoles;  /**< Mapping of usernames to roles */
    QString m_currentUsername;              /**< Current player's username */
    QString m_currentRole;                  /**< Current player's role */

    // Game board components
    QVBoxLayout* gameVerticalLayout;  /**< Vertical layout for the game */
    QHBoxLayout* mainLayout;          /**< Main horizontal layout */
    QGridLayout* m_grid;              /**< Grid layout for the game board */
    QLabel* m_playerInfoLabel;        /**< Label showing player information */
    QLabel* m_turnLabel;              /**< Label showing current turn */
    QList<QPushButton*> m_tiles;      /**< List of clickable word tiles */
    SpymasterHint* hint;              /**< Widget for spymaster to enter hints */
    OperatorGuess* guess;             /**< Widget for operators to make guesses */

    QLabel* blueCardText;             /**< Label showing blue cards remaining */
    QLabel* redCardText;              /**< Label showing red cards remaining */

    // Game state
    QStringList m_words;             /**< List of words used in the game */
    QStringList m_tileColors;        /**< List of card colors/teams */
    QStringList m_turnOrder;         /**< Order of player turns */
    int m_currentTurnIndex;          /**< Index of the current turn */

    // Cards remaining
    int redCardsRemaining;           /**< Number of red team cards left */
    int blueCardsRemaining;          /**< Number of blue team cards left */

    /**
     * @brief Sets up the user interface for the game board.
     * 
     * @details Creates and arranges all UI components including the grid,
     * information displays, and player controls.
     * 
     * @author Group 9
     */
    void setupUI();
    
    /**
     * @brief Sets up the game board with cards and initial state.
     * 
     * @details Initializes the game grid with words and card types,
     * and configures the initial display state of all cards.
     * 
     * @author Group 9
     */
    void setupBoard();
    
    /**
     * @brief Initializes the words for the game.
     * 
     * @details Loads or generates the set of words to be used for
     * the current game session.
     * 
     * @author Group 9
     */
    void initializeWords();
    
    /**
     * @brief Initializes the board colors/teams for the game.
     * 
     * @details Assigns team affiliations (colors) to each card on the board,
     * ensuring proper distribution of red, blue, neutral, and assassin cards.
     * 
     * @author Group 9
     */
    void initializeBoardColors();
    
    /**
     * @brief Sends the initial game state to all connected clients.
     * 
     * @details Broadcasts the starting configuration of the game board
     * to all players to ensure synchronization at game start.
     * 
     * @author Group 9
     */
    void sendInitialGameState();
    
    /**
     * @brief Loads word list from a file.
     * 
     * @details Reads the dictionary of possible words from a data file
     * to use for populating the game board.
     * 
     * @author Group 9
     */
    void loadWordsFromFile();
    
    /**
     * @brief Generates the game grid layout.
     * 
     * @details Creates the visual grid of cards with words and
     * configures their initial appearance and behavior.
     * 
     * @author Group 9
     */
    void generateGameGrid();
    
    /**
     * @brief Checks if the game has ended.
     * 
     * @details Evaluates the current game state to determine if
     * either team has won or if the game should continue.
     * 
     * @author Group 9
     */
    void checkGameEnd();
    
    /**
     * @brief Processes a chat message from a player.
     * 
     * @details Handles incoming chat messages, displays them in the chat box,
     * and checks for any game-related commands.
     * 
     * @param playerName The name of the player who sent the message.
     * @param message The content of the chat message.
     * 
     * @author Group 9
     */
    void processChatMessage(const QString& playerName, const QString& message);
    
    /**
     * @brief Reveals a tile on the game board.
     * 
     * @details Updates a card's state to revealed, shows its team affiliation,
     * and updates the game state accordingly.
     * 
     * @param row The row of the tile in the grid.
     * @param col The column of the tile in the grid.
     * @param broadcast Whether to broadcast this action to other players.
     * 
     * @author Group 9
     */
    void revealTile(int row, int col, bool broadcast = true);
    
    /**
     * @brief Advances to the next turn in the game.
     * 
     * @details Updates the current turn state and notifies players
     * of whose turn it is now.
     * 
     * @author Group 9
     */
    void advanceTurn();
    
    /**
     * @brief Advances the turn after a spymaster provides a hint.
     * 
     * @details Processes a spymaster's hint, displays it to all players,
     * and changes the turn to the corresponding team's operator.
     * 
     * @param hint The word hint provided by the spymaster.
     * @param number The number of cards the hint relates to.
     * 
     * @author Group 9
     */
    void advanceTurnSpymaster(const QString& hint, int number);
    
    /**
     * @brief Updates the turn display for all players.
     * 
     * @details Refreshes the UI elements that show whose turn it is
     * and what actions are available.
     * 
     * @author Group 9
     */
    void updateTurnDisplay();
    
    /**
     * @brief Sends a message to all connected players.
     * 
     * @details Broadcasts a network message to all players in the game.
     * 
     * @param message The message to broadcast.
     * 
     * @author Group 9
     */
    void sendToAll(const QString& message);
    
    /**
     * @brief Displays a hint to all players.
     * 
     * @details Updates the UI to show the current hint and related number
     * provided by a spymaster.
     * 
     * @param hint The word hint to display.
     * @param number The number associated with the hint.
     * 
     * @author Group 9
     */
    void displayHint(const QString& hint, int number);
    
    /**
     * @brief Ends the current game session.
     * 
     * @details Finalizes the game, shows the winning team, and prepares
     * for a possible new game.
     * 
     * @param message The end game message to display.
     * 
     * @author Group 9
     */
    void endGame(const QString& message);

    static const int GRID_SIZE = 5;              /**< Size of the game grid (5x5) */
    Card gameGrid[GRID_SIZE][GRID_SIZE];         /**< 2D array of game cards */
    QStringList wordList;                        /**< List of available words */
    QPushButton* cards[GRID_SIZE][GRID_SIZE];    /**< 2D array of card buttons */
    QLabel* currentHint;                         /**< Label showing current hint */
    QString correspondingNumber;                 /**< Number associated with current hint */

    /**
     * @brief Checks if it's the current player's turn.
     * 
     * @details Determines whether the current player is allowed to
     * perform actions based on the current turn state.
     * 
     * @return True if it's the current player's turn, false otherwise.
     * 
     * @author Group 9
     */
    bool isMyTurn() const;
    
    /**
     * @brief Gets the team of the current player.
     * 
     * @details Returns a string representing which team (red or blue)
     * the current player belongs to.
     * 
     * @return A string containing the team name.
     * 
     * @author Group 9
     */
    QString getMyTeam() const;
    
    /**
     * @brief Gets the CSS style for a specific card color.
     * 
     * @details Returns the styling information for rendering cards
     * of a particular team/color.
     * 
     * @param color The color/team to get the style for.
     * @return A string containing CSS style information.
     * 
     * @author Group 9
     */
    QString getColorStyle(const QString& color) const;
};

#endif // MULTIBOARD_H