#include "spymasterhint.h"

SpymasterHint::SpymasterHint(QWidget* parent) : QWidget(parent) {
    // Set up the layout
    QHBoxLayout* layout = new QHBoxLayout(this);

    // Create the LineEdit, SpinBox, and Button widgets
    hintLineEdit = new QLineEdit(this);
    hintLineEdit->setPlaceholderText("Type your clue here");
    hintLineEdit->setFixedSize(380, 40);
    hintLineEdit->setStyleSheet("text-transform: uppercase; padding: 5px; font-size: 16px;");

    // Set up a validator to only allow text input and connect the textChanged signal to textToUppercase
    textValidator = new QRegularExpressionValidator(QRegularExpression("[A-Za-z]+"), this);
    hintLineEdit->setValidator(textValidator);
    connect(hintLineEdit, &QLineEdit::textChanged, this, &SpymasterHint::textToUppercase);

    // Create a SpinBox for the number of words
    numberSpinBox = new QSpinBox(this);
    numberSpinBox->setRange(0, 9);
    numberSpinBox->setSpecialValueText("∞"); // Special value for unlimited guesses
    numberSpinBox->setFixedSize(75, 40);

    // Set up the SpinBox to only allow one digit input
    QLineEdit* lineEditor = numberSpinBox->findChild<QLineEdit*>();
    if (lineEditor) {
        lineEditor->setMaxLength(1);
    }

    // Create a button to submit the hint
    giveClueButton = new QPushButton("Give Clue", this);
    giveClueButton->setFixedSize(135, 40);
    giveClueButton->setStyleSheet(
        "QPushButton { font-weight: bold; font-size: 16px; color: white; border-radius: 15px; "
        "background-color: rgb(54, 182, 58); }"
        "QPushButton:hover { background-color: rgb(54, 212, 58); }");
    giveClueButton->setEnabled(false);
    giveClueButton->setCursor(Qt::PointingHandCursor);

    // Add the widgets to the layout
    layout->addWidget(hintLineEdit);
    layout->addWidget(numberSpinBox);
    layout->addWidget(giveClueButton);
    setLayout(layout);

    // Connect the button to the submitHint slot and the LineEdit to the updateButtonClickable slot
    connect(giveClueButton, &QPushButton::clicked, this, &SpymasterHint::submitHint);
    connect(hintLineEdit, &QLineEdit::textChanged, this, &SpymasterHint::updateButtonClickable);
}

SpymasterHint::~SpymasterHint() {
    // Deconstructor for the SpymasterHint class, does nothing
}

// Submit the hint to the game board
void SpymasterHint::submitHint() {
    QString hint = hintLineEdit->text();
    int number = numberSpinBox->value();
    emit hintSubmitted(hint, number);
    hintLineEdit->clear();
    numberSpinBox->setSpecialValueText("∞");
}

// Update the button clickable state based on the hint text
void SpymasterHint::updateButtonClickable() {
    giveClueButton->setEnabled(!hintLineEdit->text().isEmpty());
}

// Convert the hint text to uppercase
void SpymasterHint::textToUppercase(const QString& text) {
    if (text != text.toUpper()) {
        hintLineEdit->setText(text.toUpper());
    }
}