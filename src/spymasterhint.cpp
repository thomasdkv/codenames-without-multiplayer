#include "spymasterhint.h"

SpymasterHint::SpymasterHint(QWidget* parent) : QWidget(parent) {
    // Set up the layout
    QHBoxLayout* layout = new QHBoxLayout(this);

    // Create the LineEdit, SpinBox, and Button widgets
    hintLineEdit = new QLineEdit(this);
    hintLineEdit->setPlaceholderText("Type your clue here");

    QSpinBox* numberSpinBox = new QSpinBox(this);
    numberSpinBox->setRange(0, 9);
    numberSpinBox->setSpecialValueText("∞"); // Special value for unlimited guesses

    QPushButton* giveClueButton = new QPushButton("Give Clue", this);

    // Add the widgets to the layout
    layout->addWidget(hintLineEdit);
    layout->addWidget(numberSpinBox);
    layout->addWidget(giveClueButton);
    setLayout(layout);

    // Connect the button to the submitHint slot
    connect(giveClueButton, &QPushButton::clicked, this, &SpymasterHint::submitHint);
}

SpymasterHint::~SpymasterHint() {
    // Let Qt handle widget deletion
}

void SpymasterHint::submitHint() {
    QString hint = hintLineEdit->text();
    int number = numberSpinBox->value();
    if (hint.isEmpty()) {
        QMessageBox::warning(this, "Missing Hint", "Please enter a hint");
        return;
    } else {
        emit hintSubmitted(hint, number);
    }
}