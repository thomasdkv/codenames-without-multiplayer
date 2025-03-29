#include "chatbox.h"

ChatBox::ChatBox(const QString& playerName, Team team, QWidget* parent) 
    : QWidget(parent), team(team), playerName(playerName) {
    // Set up the layout and UI
    QVBoxLayout* layout = new QVBoxLayout(this);
    chatDisplay = new QTextEdit(this);
    chatDisplay->document()->setDocumentMargin(0);
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
    QColor bgColor = (team == BLUE_TEAM) ? QColor(103, 103, 255, 51) : QColor(255, 103, 103, 51);
    
    // Create a new text cursor at the end of the document to insert the message
    QTextCursor cursor = chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    // Create a text block format with team background color
    QTextBlockFormat blockFormat;
    blockFormat.setBackground(bgColor);
    blockFormat.setBottomMargin(2);
    blockFormat.setTopMargin(0); 
    
    // Check if this is the first message
    if (chatDisplay->document()->isEmpty()) {
        cursor.setBlockFormat(blockFormat);
    } else {
        // Insert a new block with this format
        cursor.insertBlock(blockFormat);
    }
    
    // Style the text with the team color and bold font, then insert the message
    QTextCharFormat charFormat;
    charFormat.setForeground(QColor(nameColor));
    charFormat.setFontWeight(QFont::Bold);
    cursor.insertText(message, charFormat);
    chatDisplay->setTextCursor(cursor);
    chatDisplay->ensureCursorVisible();
}

void ChatBox::addPlayerMessage(const QString& playerName, const QString& message) {
    // Create clean HTML-formatted message
    QString formattedMessage = QString("<b>&lt;%1&gt;</b> %2")
                              .arg(playerName)
                              .arg(message.toHtmlEscaped());
    chatDisplay->append(formattedMessage);

    // Create a new text cursor at the end of the document to insert the message
    QTextCursor cursor = chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);

    // Create a text block format with bottom margin
    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(2); 
    cursor.setBlockFormat(blockFormat);
    chatDisplay->setTextCursor(cursor);
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

void ChatBox::setPlayerName(const QString& name) {
    playerName = name;
}

void ChatBox::clearChat() {
    // Clear the chat display
    chatDisplay->clear();
}