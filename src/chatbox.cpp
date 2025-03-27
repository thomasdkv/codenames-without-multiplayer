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