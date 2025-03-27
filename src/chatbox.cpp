#include "chatbox.h"

ChatBox::ChatBox(const QString& playerName, QWidget* parent) : QWidget(parent), playerName(playerName) {
    // Set up the layout and UI
    QVBoxLayout* layout = new QVBoxLayout(this);
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    layout->addWidget(chatDisplay);

    // Set up the chat input layout
    QHBoxLayout* inputLayout = new QHBoxLayout();
    chatInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    inputLayout->addWidget(chatInput);
    inputLayout->addWidget(sendButton);
    layout->addLayout(inputLayout);

    // Connect signals and slots
    connect(sendButton, &QPushButton::clicked, this, &ChatBox::sendMessage);
    connect(chatInput, &QLineEdit::returnPressed, this, &ChatBox::sendMessage);
    setLayout(layout);
}

ChatBox::~ChatBox() {
    // Deconstructor for the ChatBox class, does nothing
}

void ChatBox::addSystemMessage(const QString& message) {
    // Add a system message to the chat display
    chatDisplay->append("<b>System: </b>" + message);
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