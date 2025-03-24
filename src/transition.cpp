#include "transition.h"

Transition::Transition (QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    messageLabel = new QLabel("P", this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    continueButton = new QPushButton("Continue", this);
    continueButton->setFixedSize(180, 40);
    continueButton->setStyleSheet(
        "QPushButton { font-weight: bold; font-size: 16px; color: white; border-radius: 15px; "
        "background-color:rgb(65, 42, 213); }"
        "QPushButton:hover { background-color: rgb(54, 35, 177); }");
    continueButton->setEnabled(true);
    continueButton->setCursor(Qt::PointingHandCursor);

    layout->addStretch();
    layout->addWidget(messageLabel);
    layout->addWidget(continueButton);
    layout->addStretch();
    setLayout(layout);

    connect(continueButton, &QPushButton::clicked, this, &Transition::continueClicked);  
}

Transition::~Transition() {
    // Deconstructor for the Transition class, does nothing
}

void Transition::setMessage(const QString& message) {
    messageLabel->setText(message);
}