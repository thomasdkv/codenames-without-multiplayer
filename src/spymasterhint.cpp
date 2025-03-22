#include "spymasterhint.h"

SpymasterHint::SpymasterHint(QWidget* parent) : QWidget(parent) {
    // Set up the layout
    QHBoxLayout* layout = new QHBoxLayout(this);

    // Create the LineEdit, SpinBox, and Button widgets
    hintLineEdit = new QLineEdit(this);
    hintLineEdit->setPlaceholderText("Type your clue here");
    hintLineEdit->setFixedSize(300, 35);
    hintLineEdit->setStyleSheet("text-transform: uppercase; padding: 5px; font-size: 16px;");

    textValidator = new QRegularExpressionValidator(QRegularExpression("[A-Za-z]+"), this);
    hintLineEdit->setValidator(textValidator);

    numberSpinBox = new QSpinBox(this);
    numberSpinBox->setRange(0, 9);
    numberSpinBox->setSpecialValueText("∞"); // Special value for unlimited guesses
    numberSpinBox->setFixedSize(50, 35);

    giveClueButton = new QPushButton("Give Clue", this);
    giveClueButton->setFixedSize(100, 35);
    giveClueButton->setStyleSheet(
        "font-weight: bold; font-size: 16px; color: white; border-radius: 5px;"
        "background-color:rgb(54, 182, 58);"
        "QPushButton:hover { background-color: rgb(119, 236, 117); }");
    giveClueButton->setEnabled(false);
    giveClueButton->setCursor(Qt::PointingHandCursor);


    // Add the widgets to the layout
    layout->addWidget(hintLineEdit);
    layout->addWidget(numberSpinBox);
    layout->addWidget(giveClueButton);
    setLayout(layout);

    // Connect the button to the submitHint slot
    connect(giveClueButton, &QPushButton::clicked, this, &SpymasterHint::submitHint);
    connect(hintLineEdit, &QLineEdit::textChanged, this, &SpymasterHint::updateButtonClickable);
}

SpymasterHint::~SpymasterHint() {
    // Deconstructor for the SpymasterHint class, does nothing
}

void SpymasterHint::submitHint() {
    QString hint = hintLineEdit->text();
    int number = numberSpinBox->value();
    emit hintSubmitted(hint, number);
    hintLineEdit->clear();
    numberSpinBox->setSpecialValueText("∞");
}

void SpymasterHint::updateButtonClickable() {
    giveClueButton->setEnabled(!hintLineEdit->text().isEmpty());
}

void SpymasterHint::textToUppercase(const QString& text) {
    if (text != text.toUpper()) {
        hintLineEdit->setText(text.toUpper());
    }
}