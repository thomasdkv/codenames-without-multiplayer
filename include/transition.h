/**
 * @file transition.h
 * @author Matthew Marbina (Group 9)
 * @brief Header file for the Transition class, which provides a UI for transitions between game states.
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef TRANSITION_H
#define TRANSITION_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

/**
 * @class Transition
 * @brief A widget for displaying a transition message and a button to continue.
 * @details This class contains a QLabel for the message and a QPushButton to continue. It emits a signal 
 * when the button is clicked.
 * @author Group 9
 */
class Transition : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the Transition class.
     * @details This constructor sets up the layout and initializes the widgets.
     * It connects the button to the continueClicked signal.
     * @param parent The parent widget.
     */
    explicit Transition(QWidget* parent = nullptr);

    /**
     * @brief Destructor for the Transition class.
     * @details This destructor cleans up the resources used by the class.
     * It does not need to explicitly delete the widgets as they are managed by Qt's parent-child system.
     */
    ~Transition();

    /**
     * @brief Sets the message to be displayed.
     * @details This function updates the text of the message label shown in the transition screen UI.
     * @param message The message text.
     */
    void setMessage(const QString& message);

signals:
    /**
     * @brief Signal emitted when the continue button is clicked.
     * @details This signal is emitted when the user clicks the continue button in the transition screen.
     * After this signal is emitted, the game can proceed to the next state.
     */
    void continueClicked();

private:
    /**
     * @brief The label that displays the transition message.
     */
    QLabel* messageLabel;

    /**
     * @brief The button that allows the user to continue.
     */
    QPushButton* continueButton;
};

#endif // TRANSITION_H