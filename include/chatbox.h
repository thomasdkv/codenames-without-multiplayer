/**
 * @file chatbox.h
 * @author Matthew Marbina (Group 9)
 * @brief Header file for the ChatBox class, which provides a UI for the chat feature in the game.
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef CHATBOX_H
#define CHATBOX_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

/**
 * @class ChatBox
 * @brief A widget for the chat feature in the game.
 * @details This class contains a QTextEdit for displaying chat messages, a QLineEdit for inputting messages,
 * and a QPushButton to send messages. It allows players to communicate with each other during the game.
 * It also includes functionality to display system messages and player messages with different styles based on 
 * operative guesses and spymaster hints for each team.
 * @author Group 9
 */
class ChatBox : public QWidget {
    Q_OBJECT

public:
    /**
     * @enum Team
     * @brief Enumeration for the two teams in the game.
     * @details This enum is used to differentiate between the two teams (red and blue) in the game.
     * It is used to style the chat messages and system messages based on the team.
     */
    enum Team {
        RED_TEAM,
        BLUE_TEAM
    };

    /**
     * @brief Constructor for the ChatBox class.
     * @details This constructor sets up the layout and initializes the widgets. 
     * It connects the button to the sendMessage slot and the LineEdit to the sendMessage slot.
     * @param playerName The name of the player using this chat box.
     * @param team The team of the player (red or blue).
     * @param parent The parent widget.
     */
    explicit ChatBox(const QString& playerName, Team team, QWidget* parent = nullptr);

    /**
     * @brief Destructor for the ChatBox class.
     * @details This destructor cleans up the resources used by the class.
     * It does not need to explicitly delete the widgets as they are managed by Qt's parent-child system.
     */
    ~ChatBox();

    /**
     * @brief Adds a system message to the chat display.
     * @details This function adds a system message to the chat box, printing the operative guesses and spymaster 
     * hints for each team. It styles the message based on the team and the type of message.
     * @param message The system message text.
     * @param team The team associated with the message (red or blue).
     */
    void addSystemMessage(const QString& message, Team team);

    /**
     * @brief Adds a player message to the chat display.
     * @details This function adds a player message to the chat box for both local play and online play.
     * @param playerName The name of the player sending the message.
     * @param message The message text.
     */
    void addPlayerMessage(const QString& playerName, const QString& message);

    /**
     * @brief Sets the player name for the chat box.
     * @details This function sets the player name for the chat box, which is used to identify the sender of messages.
     * @param name The name of the player.
     */
    void setPlayerName(const QString& name);

    /**
     * @brief Clears the chat display.
     * @details This function clears all messages from the chat display so the chat is empty for new games.
     */
    void clearChat();

    /**
     * @brief Displays a message when the guess limit is reached.
     * @details This function displays a message indicating that the operative has reached the limit for their guesses, 
     * meaning they cannot make any more guesses and must end their turn.
     */
    void limitReachedMessage();

public slots:
    /**
     * @brief Sends a message from the chat input.
     * @details This function retrieves the text from the chat input field and emits a signal to send the message.
     * It also clears the input field after sending the message.
     */
    void sendMessage();
signals:
    /**
     * @brief Signal emitted when a message is sent.
     * @details This signal is emitted when the user sends a message from the chat input field.
     * It carries the player name and the message text as parameters.
     * @param playerName The name of the player sending the message.
     * @param message The message text.
     */
    void massSend(const QString& playerName, const QString& message);

private:
    /**
     * @brief The team of the player using this chat box.
     */
    Team team;

    /**
     * @brief The text edit widget for displaying chat messages.
     */
    QTextEdit* chatDisplay;

    /**
     * @brief The line edit widget for inputting chat messages.
     */
    QLineEdit* chatInput;

    /**
     * @brief The button to send chat messages.
     */
    QPushButton* sendButton;

    /**
     * @brief The name of the player using this chat box.
     */
    QString playerName;
};

#endif // CHATBOX_H