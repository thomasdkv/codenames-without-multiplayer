/**
 * @file multipregame.h
 * @brief Header file for the MultiPregame class, which implements the pre-game lobby for multiplayer games.
 * @author Group 9
 */

#ifndef MULTIPREGAME_H
#define MULTIPREGAME_H

#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QLoggingCategory>

/**
 * @class MultiPregame
 * @brief A widget that implements the pre-game lobby for multiplayer games.
 * 
 * @details The MultiPregame class manages the pre-game setup phase where players join the lobby,
 * select their roles, and prepare for the game to start. It handles both host and client
 * functionality, manages player connections, role assignments, and transitions to the game.
 * 
 * @author Group 9
 */
class MultiPregame : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Constructor for the MultiPregame class when acting as a host.
     * 
     * @details Initializes the pre-game lobby for a host player who is creating a new game.
     * Sets up the server to accept client connections and manages the player list.
     * 
     * @param server Pointer to the WebSocket server for hosting the game.
     * @param username The username of the host player.
     * @param parent Optional parent widget.
     * 
     * @author Group 9
     */
    MultiPregame(QWebSocketServer* server, const QString& username, QWidget* parent = nullptr);
    
    /**
     * @brief Constructor for the MultiPregame class when acting as a client.
     * 
     * @details Initializes the pre-game lobby for a client player who is joining an existing game.
     * Sets up the connection to the host server and prepares the player for role selection.
     * 
     * @param socket Pointer to the WebSocket connection to the host.
     * @param username The username of the client player.
     * @param parent Optional parent widget.
     * 
     * @author Group 9
     */
    MultiPregame(QWebSocket* socket, const QString& username, QWidget* parent = nullptr);
    
    /**
     * @brief Destructor for the MultiPregame class.
     * 
     * @details Cleans up resources, including network connections and UI components.
     * 
     * @author Group 9
     */
    ~MultiPregame();
    
    /**
     * @brief Clears the user interface elements.
     * 
     * @details Removes all UI elements from the widget and prepares it for rebuilding
     * or transition to another state.
     * 
     * @author Group 9
     */
    void clearUI();

signals:
    /**
     * @brief Signal emitted when returning to the multiplayer main menu.
     * 
     * @details Indicates that the player wants to exit the pre-game lobby
     * and return to the multiplayer main menu.
     */
    void backToMultiMain();
    
    /**
     * @brief Signal emitted when entering the pre-game setup as a host.
     * 
     * @details Used for transitioning to or refreshing the pre-game lobby
     * with the current player as the host.
     * 
     * @param server Pointer to the WebSocket server instance.
     * @param username The username of the host player.
     */
    void enterPregameAsHost(QWebSocketServer* server, const QString& username);
    
    /**
     * @brief Signal emitted when entering the pre-game setup as a client.
     * 
     * @details Used for transitioning to or refreshing the pre-game lobby
     * with the current player as a client.
     * 
     * @param socket Pointer to the client's WebSocket connection.
     * @param username The username of the client player.
     */
    void enterPregameAsClient(QWebSocket* socket, const QString& username);

public slots:
    /**
     * @brief Handles a new client connection to the game server.
     * 
     * @details Accepts a new WebSocket connection from a client,
     * adds them to the player list, and updates all connected clients.
     * Only used when this instance is the host.
     * 
     * @author Group 9
     */
    void onNewConnection();
    
    /**
     * @brief Processes an incoming message from a WebSocket.
     * 
     * @details Handles and responds to various message types from
     * connected clients or the host server, including role selections
     * and game start notifications.
     * 
     * @param message The message received from the WebSocket.
     * 
     * @author Group 9
     */
    void processMessage(const QString& message);
    
    /**
     * @brief Handles a WebSocket disconnection.
     * 
     * @details Cleans up resources and updates the lobby state when
     * a client disconnects from the server or the server disconnects.
     * 
     * @author Group 9
     */
    void socketDisconnected();
    
    /**
     * @brief Starts the game.
     * 
     * @details Initiates the game when all players are ready and roles are assigned.
     * Sends game start notification to all clients and transitions to the game board.
     * Only the host can trigger this action.
     * 
     * @author Group 9
     */
    void startGame();

private:
    /**
     * @brief Resets the UI state to its initial condition.
     * 
     * @details Clears player selections, role assignments, and readiness states,
     * preparing the UI for a fresh lobby state.
     * 
     * @author Group 9
     */
    void resetUIState();
    
    /**
     * @brief Sets up the user interface for the pre-game lobby.
     * 
     * @details Creates and arranges all UI components including the player list,
     * role selection controls, and game start button.
     * 
     * @author Group 9
     */
    void setupUI();
    
    /**
     * @brief Sends an updated lobby state to all connected clients.
     * 
     * @details Broadcasts the current player list, role assignments, and
     * readiness states to all connected clients to keep everyone synchronized.
     * 
     * @author Group 9
     */
    void sendLobbyUpdate();
    
    /**
     * @brief Handles a role selection message from a player.
     * 
     * @details Processes a player's request to select a specific role,
     * validates the selection, updates the lobby state, and notifies all clients.
     * 
     * @param message The role selection message.
     * @param sender Pointer to the WebSocket of the player making the selection.
     * 
     * @author Group 9
     */
    void handleRoleSelection(const QString& message, QWebSocket* sender);
    
    /**
     * @brief Handles the game start transition.
     * 
     * @details Sets up the necessary data and transitions to the game board
     * when the game is started by the host.
     * 
     * @param isHost Boolean indicating if this instance is the host.
     * @param server Pointer to the WebSocket server (if host).
     * @param clients List of connected client WebSockets.
     * @param clientSocket Pointer to this player's WebSocket (if client).
     * @param playerRoles Hash mapping player names to their selected roles.
     * 
     * @author Group 9
     */
    void gameStarted(bool isHost, QWebSocketServer* server, const QList<QWebSocket*>& clients, QWebSocket* clientSocket, const QHash<QString, QString>& playerRoles);
    
    /**
     * @brief Displays the pre-game lobby interface.
     * 
     * @details Shows the pre-game lobby UI and updates it with the current
     * player list and role selections.
     * 
     * @author Group 9
     */
    void showPregame();

    /** @brief WebSocket server for hosting (nullptr for clients) */
    QWebSocketServer* m_server = nullptr;      
    /** @brief Client WebSocket connection (nullptr for hosts) */
    QWebSocket* m_clientSocket = nullptr;    
    /** @brief List of connected client WebSockets */
    QList<QWebSocket*> m_clients;
    /** @brief Mapping of WebSockets to player usernames */     
    QMap<QWebSocket*, QString> m_usernames;   
    /** @brief Mapping of WebSockets to player roles */
    QMap<QWebSocket*, QString> m_roles;      
    /** @brief Mapping of WebSockets to player readiness state */ 
    QMap<QWebSocket*, bool> m_checked;        
    
    /** @brief Widget displaying the list of connected players */
    QListWidget* playerList;  
    /** @brief Current player's username */                
    QString m_username;    
    /** @brief Boolean indicating if this instance is the host */                    
    bool m_isHost;                            
};

#endif // MULTIPREGAME_H