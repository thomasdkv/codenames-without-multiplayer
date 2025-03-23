#include "operatorguess.h"

OperatorGuess::OperatorGuess(QWidget* parent) : QWidget(parent) {
    // Set up the layout
    QHBoxLayout* layout = new QHBoxLayout(this);

 
    // Create a button to submit the guess
    submitGuessButton = new QPushButton("Finish Guessing", this);
    submitGuessButton->setFixedSize(135, 40);
    submitGuessButton->setStyleSheet(
        "QPushButton { font-weight: bold; font-size: 16px; color: white; border-radius: 15px; "
        "background-color: rgb(54, 182, 58); }"
        "QPushButton:hover { background-color: rgb(54, 212, 58); }");
    submitGuessButton->setEnabled(true);
    submitGuessButton->setCursor(Qt::PointingHandCursor);
    connect(submitGuessButton, &QPushButton::clicked, this, &OperatorGuess::submitGuess);

    // Add the widgets to the layout
    layout->addWidget(guessLineEdit);
    layout->addWidget(submitGuessButton);
    setLayout(layout);


}

OperatorGuess::~OperatorGuess() {
    // Destructor does nothing
}

// Submit the guess to the game board
void OperatorGuess::submitGuess() {
    qDebug() << "Submitted guess";
    emit guessSubmitted();
}

// Update the button clickable state based on the guess text
void OperatorGuess::updateButtonClickable() {
    submitGuessButton->setEnabled(!guessLineEdit->text().isEmpty());
}

// Convert the guess text to uppercase
void OperatorGuess::textToUppercase(const QString& text) {
    if (text != text.toUpper()) {
        guessLineEdit->setText(text.toUpper());
    }
}