#include "transition.h"

Transition::Transition (QWidget* parent) : QWidget(parent) {
    // Set up the layout and UI
    QVBoxLayout* layout = new QVBoxLayout(this);
    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    // Set up the continue button, style it, and set its properties
    continueButton = new QPushButton("Continue", this);
    continueButton->setFixedSize(180, 40);
    continueButton->setStyleSheet(
        "QPushButton { font-weight: bold; font-size: 16px; color: white; border-radius: 15px; "
        "background-color:rgb(65, 42, 213); }"
        "QPushButton:hover { background-color: rgb(54, 35, 177); }");
    continueButton->setEnabled(true);
    continueButton->setCursor(Qt::PointingHandCursor);

    // Add the widgets to the layout and set the layout properties
    layout->addStretch();
    layout->addWidget(messageLabel);
    layout->addWidget(continueButton);
    layout->addStretch();
    setLayout(layout);
    
    // Connect the button's clicked signal to the continueClicked signal
    connect(continueButton, &QPushButton::clicked, this, &Transition::continueClicked);  
}

Transition::~Transition() {
    // Deconstructor for the Transition class, does nothing
}

// Function to set the message displayed in the transition screen
void Transition::setMessage(const QString& message) {
    messageLabel->setText(message);
}