#include "spymasterhint.h"

SpymasterHint::SpymasterHint(QWidget* parent) : QWidget(parent) {
    // Set up the layout
    QHBoxLayout* layout = new QHBoxLayout(this);

    // Create the LineEdit, SpinBox, and Button widgets
    QLineEdit* enterClue = new QLineEdit(this);
    enterClue->setPlaceholderText("Type your clue here");

    QSpinBox* enterNumber = new QSpinBox(this);
    enterNumber->setRange(0, 9);
    enterNumber->setSpecialValueText("∞"); // Special value for unlimited guesses

    QPushButton* giveClueButton = new QPushButton("Give Clue", this);

    // Add the widgets to the layout
    layout->addWidget(enterClue);
    layout->addWidget(enterNumber);
    layout->addWidget(giveClueButton);
    setLayout(layout);

    // Connect the button to the submitHint slot
    connect(giveClueButton, &QPushButton::clicked, this, &SpymasterHint::submitHint);

    // Store the widgets in the class
    hintLineEdit = enterClue; 
    numberSpinBox = enterNumber;
    giveClueButton = giveClueButton;
}

SpymasterHint::~SpymasterHint() {
    // Let Qt handle widget deletion
}

void SpymasterHint::submitHint() {
    QString hint = hintLineEdit->text();
    int number = numberSpinBox->value();
    if (hint.isEmpty()) {
        layout()->addWidget(new QLabel("Please enter a hint"));
        return;
    } else {
        emit hintSubmitted(hint, number);
    }
}