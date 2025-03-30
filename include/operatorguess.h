/**
 * @file operatorguess.h
 * @brief Header file for the OperatorGuess class, which handles operator guessing interface.
 * @author Group 9
 */

#ifndef OPERATORGUESS_H
#define OPERATORGUESS_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpressionValidator>

/**
 * @class OperatorGuess
 * @brief A widget that provides the interface for operators to submit guesses during gameplay.
 * 
 * @details The OperatorGuess class provides a simple UI for team operators to submit
 * their guesses during their turn. It consists of a button that the operator can
 * click to indicate they have made a guess on the game board.
 * 
 * @author Group 9
 */
class OperatorGuess : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the OperatorGuess class.
     * 
     * @details Initializes the operator guess interface with a submit button.
     * Sets up the UI components and connections.
     * 
     * @param parent Optional parent widget.
     * 
     * @author Group 9
     */
    explicit OperatorGuess(QWidget* parent = nullptr);
    
    /**
     * @brief Destructor for the OperatorGuess class.
     * 
     * @details Cleans up resources used by the OperatorGuess widget.
     * 
     * @author Group 9
     */
    ~OperatorGuess();

    /**
     * @brief Resets the operator guess interface.
     * 
     * @details Resets the state of the interface to prepare it for a new turn.
     * This may involve enabling/disabling the button or clearing any internal state.
     * 
     * @author Group 9
     */
    void reset();

signals:
    /**
     * @brief Signal emitted when a guess is submitted.
     * 
     * @details Indicates that the operator has clicked the submit button
     * to register their guess on the game board.
     */
    void guessSubmitted();

private slots:
    /**
     * @brief Handles the submission of a guess.
     * 
     * @details Processes the operator's action when they click the submit button
     * to indicate they have made a guess. Emits the guessSubmitted signal.
     * 
     * @author Group 9
     */
    void submitGuess();

private:
    QPushButton* submitGuessButton; /**< Button for submitting a guess */
};

#endif // OPERATORGUESS_H