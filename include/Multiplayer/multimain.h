/**
 * @file multimain.h
 * @brief Header file for the MultiMain class, which implements the main
 * multiplayer game lobby interface.
 * @author Your Name
 */

#ifndef MULTIMAIN_H
#define MULTIMAIN_H

#include <QComboBox>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QWidget>

#include "Multiplayer/multiboard.h"
#include "Multiplayer/multipregame.h"
#include "user.h"

/**
 * @class MultiMain
 * @brief A widget that implements the main multiplayer lobby for creating and
 * joining game rooms.
 *
 * @details The MultiMain class provides the interface for players to either
 * create a new game room as a host or join an existing game room as a client.
 * It manages WebSocket connections for multiplayer functionality and handles
 * the transition to the pre-game setup screen.
 *
 * @author Your Name
 */
class MultiMain : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Constructor for the MultiMain class.
   *
   * @details Initializes the multiplayer lobby interface with buttons for
   * creating and joining game rooms. Sets up the UI components and prepares
   * network connections.
   *
   * @param parent Optional parent widget.
   *
   * @author Your Name
   */
  explicit MultiMain(QWidget* parent = nullptr);

  /**
   * @brief Destructor for the MultiMain class.
   *
   * @details Cleans up resources, including network connections and UI
   * components.
   *
   * @author Your Name
   */
  ~MultiMain();

  /**
   * @brief Displays the main lobby window.
   *
   * @details Makes the multiplayer lobby interface visible and sets up initial
   * state.
   *
   * @author Your Name
   */
  void showMainWindow();

 signals:
  /**
   * @brief Signal emitted when returning to the application's main window.
   *
   * @details Indicates that the player wants to exit the multiplayer lobby
   * and return to the main application window.
   */
  void backToMainWindow();

  /**
   * @brief Signal emitted when entering the pre-game setup as a host.
   *
   * @details Triggered when a player creates a new game room and transitions
   * to the pre-game setup screen as the host.
   *
   * @param server Pointer to the WebSocket server instance.
   * @param username The username of the host player.
   */
  void enterPregameAsHost(QWebSocketServer* server, const QString& username);

  /**
   * @brief Signal emitted when entering the pre-game setup as a client.
   *
   * @details Triggered when a player joins an existing game room and
   * transitions to the pre-game setup screen as a client.
   *
   * @param socket Pointer to the client's WebSocket connection.
   * @param username The username of the client player.
   */
  void enterPregameAsClient(QWebSocket* socket, const QString& username);

 private slots:
  /**
   * @brief Opens the main application window.
   *
   * @details Handler for returning to the main application window from the
   * multiplayer lobby.
   *
   * @author Your Name
   */
  void openMainWindow();

  /**
   * @brief Handles the user clicking the "Create Room" button.
   *
   * @details Creates a new game room with the current player as host,
   * initializes the WebSocket server, and transitions to the pre-game setup.
   *
   * @author Your Name
   */
  void onCreateRoomClicked();

  /**
   * @brief Handles the user clicking the "Join Room" button.
   *
   * @details Connects to an existing game room as a client,
   * establishes a WebSocket connection to the host, and
   * transitions to the pre-game setup.
   *
   * @author Your Name
   */
  void onJoinRoomClicked();

  /**
   * @brief Handles a new client connection to the game server.
   *
   * @details Accepts a new WebSocket connection from a client
   * and sets up the communication channels.
   *
   * @author Your Name
   */
  void onNewConnection();

  /**
   * @brief Processes an incoming text message from a WebSocket.
   *
   * @details Handles and responds to various message types from
   * connected clients or the host server.
   *
   * @param message The text message received from the WebSocket.
   *
   * @author Your Name
   */
  void processTextMessage(QString message);

  /**
   * @brief Handles a WebSocket disconnection.
   *
   * @details Cleans up resources and updates the lobby state when
   * a client disconnects from the server.
   *
   * @author Your Name
   */
  void socketDisconnected();

  /**
   * @brief Handles successful connection to a host server.
   *
   * @details Processes actions to take when a client successfully
   * connects to a game room host.
   *
   * @author Your Name
   */
  void onConnected();

  /**
   * @brief Handles disconnection from a host server.
   *
   * @details Processes actions to take when a client is disconnected
   * from a game room host.
   *
   * @author Your Name
   */
  void onDisconnected();

 private:
  // Network members
  /** @brief WebSocket server for hosting game rooms */
  QWebSocketServer* m_server = nullptr;
  /** @brief Client WebSocket for joining rooms */
  QWebSocket* m_clientSocket = nullptr;
  /** @brief List of connected client WebSockets */
  QList<QWebSocket*> m_clients;
  /** @brief Mapping of WebSockets to player usernames */
  QMap<QWebSocket*, QString> m_usernames;
  /** @brief Current player's username */
  QString m_username;

  // UI members
  /** @brief Title label for the multiplayer lobby */
  QLabel* titleLabel;
  /** @brief Button for creating a new game room */
  QPushButton* createRoomButton;
  /** @brief Button for joining an existing game room */
  QPushButton* joinRoomButton;
  /** @brief Button for returning to the main window */
  QPushButton* backButton;

  /**
   * @brief Updates the list of available game lobbies.
   *
   * @details Refreshes the UI with the current list of available
   * game rooms that players can join.
   *
   * @author Your Name
   */
  void updateLobbyList();

  /**
   * @brief Sends the current lobby list to all connected clients.
   *
   * @details Broadcasts an updated list of available game rooms
   * to all clients connected to this server.
   *
   * @author Your Name
   */
  void sendLobbyListToAll();
};

#endif  // MULTIMAIN_H