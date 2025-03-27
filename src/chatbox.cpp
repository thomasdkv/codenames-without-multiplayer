#include "chatbox.h"

ChatBox::ChatBox(const QString& playerName, Team team, QWidget* parent) 
    : QWidget(parent), team(team), playerName(playerName) {
    // Set up the layout and UI
    QVBoxLayout* layout = new QVBoxLayout(this);
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    layout->addWidget(chatDisplay);

    // Set up the chat input layout
    QHBoxLayout* textArea = new QHBoxLayout();
    chatInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    textArea->addWidget(chatInput);
    textArea->addWidget(sendButton);
    layout->addLayout(textArea);

    // Connect signals and slots
    connect(sendButton, &QPushButton::clicked, this, &ChatBox::sendMessage);
    connect(chatInput, &QLineEdit::returnPressed, this, &ChatBox::sendMessage);
    setLayout(layout);
}

ChatBox::~ChatBox() {
    // Deconstructor for the ChatBox class, does nothing
}

void ChatBox::addSystemMessage(const QString& message, Team team) {
    QString nameColor = (team == BLUE_TEAM) ? "blue" : "red";
    QString messageBackgroundColor = (team == BLUE_TEAM) ? "rgba(0, 0, 255, 0.2)" : "rgba(255, 0, 0, 0.2)";

    // Format the system message with the appropriate color
    QString formattedMessage = "<span style='color:" + nameColor + "; background-color:" + messageBackgroundColor + 
        "; padding: 2px; border-radius: 5px;'>";
    formattedMessage += message + "</span>";

    chatDisplay->append(formattedMessage);
}

void ChatBox::addPlayerMessage(const QString& playerName, const QString& message) {
    // Add a player message to the chat display
    chatDisplay->append("<b>" + playerName + ": </b>" + message);
}

void ChatBox::sendMessage() {
    // Get the text from the chat input
    QString message = chatInput->text().trimmed();
    if (!message.isEmpty()) {
        // Add the player's message to the chat display
        addPlayerMessage(playerName, message);
        chatInput->clear();
    }

    // Multiplayer functionality 

}