/**
 * @file gameboard.h
 * @brief Header file for the GameBoard class, which implements a game board for the Spy Master game.
 * @author Group 9
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QStringList>
#include <QPushButton>
#include <QStackedLayout>
#include "spymasterhint.h"
#include "operatorguess.h"
#include "transition.h"
#include "chatbox.h"
#include "user.h"

/**
 * @class GameBoard
 * @brief A class representing the game board for the Spy Master game.
 * 
 * @details The GameBoard class is responsible for displaying the game board and handling user interactions.
 * It includes methods for loading words from a file, generating the game grid, setting up the UI, card clicks, card reveals
 * and turns, and handling game end conditions. The game board also includes a stacked layout for transitions between screens.
 * Codenames is a game which involves two teams (red and blue) with spymasters giving hints and operators making guesses.
 * 
 * @author Group 9
*/

class GameBoard : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the GameBoard class.
     * 
     * @details Initializes the game board with the provided team names and sets up the UI.
     * It also loads words from a file and generates the game grid which is displayed on the UI.
     * 
     * @param redSpyMaster The name of the red team's spymaster.
     * @param redOperative The name of the red team's operative.
     * @param blueSpyMaster The name of the blue team's spymaster.
     * @param blueOperative The name of the blue team's operative.
     * @param parent Optional parent widget.
     * 
     * @author Group 9
    */
    explicit GameBoard(const QString& redSpyMaster, const QString& redOperative,
                      const QString& blueSpyMaster, const QString& blueOperative,
                      QWidget* parent = nullptr);
    
    /**
     * @brief Destructor for the GameBoard class.
     * 
     * @details Cleans up resources used by the game board.
     * 
     * @author Group 9
    */
    ~GameBoard();
    
    /**
     * @brief Sets the names of the red team's spymaster and operative.
     * 
     * @details Sets the names of the red team's spymaster and operative. 
     * 
     * @param name The name of the red team's spymaster.
     * 
     * @author Group 9
    */
    void setRedSpyMasterName(const QString& name);

    /**
     * @brief Sets the names of the red team's operative.
     * 
     * @details Sets the names of the red team's operative and updates the team labels.
     * 
     * @param name The name of the red team's operative.
     * 
     * @author Group 9
    */
    void setRedOperativeName(const QString& name);

    /**
     * @brief Sets the names of the blue team's spymaster and operative.
     * 
     * @details Sets the names of the blue team's spymaster and operative and updates the team labels.
     * 
     * @param name The name of the blue team's spymaster.
     * 
     * @author Group 9
    */
    void setBlueSpyMasterName(const QString& name);

    /**
     * @brief Sets the names of the blue team's operative.
     * 
     * @details Sets the names of the blue team's operative and updates the team labels.
     * 
     * @param name The name of the blue team's operative.
     * 
     * @author Group 9
    */
    void setBlueOperativeName(const QString& name);

    /**
     * @brief Updates the labels displaying team information.
     * 
     * @details Updates the labels displaying team information, such as team names and scores.
     * 
     * @author Group 9
    */
    void updateTeamLabels();

signals:
    /**
     * @brief Emitted when the game ends.
     * 
     * @details Signals that the game has ended and the game board should be closed.
     * 
     * @author Group 9
    */
    void gameEnded();

public slots:
    /** 
     * @brief Displays the game board.
     * 
     * @details Displays the game board and sets up the UI.
     * 
     * @author Group 9
    */
    void show();

    /**
     * @brief Displays a hint on the game board.
     * 
     * @details Displays a hint on the game board for the current turn and updates the UI.
     * 
     * @param hint The hint to be displayed.
     * @param number The number of words associated with the hint.
     * 
     * @author Group 9
    */
    void displayHint(const QString& hint, int number);

    /**
     * @brief Displays a guess on the game board.
     * 
     * @details Displays a guess on the game board for the current turn and updates the UI.
     * 
     * @author Group 9
    */
    void displayGuess();

private:
    /**
     * @brief Loads words from a file and generates the game grid.
     * 
     * @details Loads words from a file and generates the game grid.
     * 
     * @author Group 9
    */
    void loadWordsFromFile();

    /**
     * @brief Generates the game grid.
     * 
     * @details Generates the game grid based on the loaded words.
     * 
     * @author Group 9
    */
    void generateGameGrid();

    /**
     * @brief Sets up the UI for the game board.
     * 
     * @details Sets up the UI for the game board, including the layout, labels, and buttons.
     * 
     * @author Group 9
    */
    void setupUI();

    /**
     * @brief Switches to the next turn.
     * 
     * @details Switches to the next turn and updates the UI.
     * 
     * @author Group 9
    */
    void nextTurn();

    /** 
     * @brief Handles a card click event.
     * 
     * @details Handles a card click event and updates the UI.
     * 
     * @param row The row of the clicked card.
     * @param col The column of the clicked card.
     * 
     * @author Group 9
    */
    void onCardClicked(int row, int col);

    /**
     * @brief Handles the continue button click event.
     * 
     * @details Handles the continue button click event and updates the UI.
     * 
     * @author Group 9
    */
    void onContinueClicked();

    /**
     * @brief Displays a transition screen.
     * 
     * @details Displays a transition screen and updates the UI.
     * 
     * @author Group 9
    */
    void showTransition();

    /**
     * @brief Updates the scores of the teams.
     * 
     * @details Updates the scores of the teams based on the current state of the game.
     * 
     * @author Group 9
    */
    void updateScores();

    /**
     * @brief Checks if the game has ended.
     * 
     * @details Checks if the game has ended based on the current state of the game.
     * 
     * @author Group 9
    */
    void checkGameEnd();

    /**
     * @brief Ends the game and displays a message.
     * 
     * @details Ends the game and displays a message.
     * 
     * @param message The message to be displayed.
     * 
     * @author Group 9
    */
    void endGame(const QString& message);

    /**
     * @brief Resets the game state.
     * 
     * @details Resets the game state to the initial state.
     * 
     * @author Group 9
    */
    void resetGame();

    /**
     * @enum CardType
     * @brief Enumeration for card types.
    */
    enum CardType {
        RED_TEAM,
        BLUE_TEAM,
        NEUTRAL,
        ASSASSIN
    };

    /**
     * @enum Turn
     * @brief Enumeration representing the different turn states in the game board.
    */
    enum Turn {
        RED_SPY,
        RED_OP,
        BLUE_SPY,
        BLUE_OP
    };

    /**
     * @brief Structure representing a card in the game grid.
     * 
     * @details Contains the word, type, and revealed status of the card.
    */
    struct Card {
        QString word;
        CardType type;
        bool revealed;
    };

    /** @brief Structure representing a turn in the game board. */
    int currentTurn;
    /** @brief The number of remaining cards for each team.*/
    int redCardsRemaining;
    /** @brief The number of remaining cards for each team.*/
    int blueCardsRemaining;

    /** @brief The maximum number of guesses allowed in a turn.*/
    int maxGuesses=0;
    /** @brief The number of guesses made in the current turn.*/
    int currentGuesses=0;

    /** @brief The names of the spymaster for the red team.*/
    QString redSpyMasterName;
    /** @brief The names of the operative for the blue team.*/
    QString redOperativeName;
    /** @brief The names of the spymaster for the blue team.*/
    QString blueSpyMasterName;
    /** @brief The names of the operative for the blue team.*/
    QString blueOperativeName;

    /** @brief The size of the game grid.*/
    static const int GRID_SIZE = 5;
    /** @brief The game grid.*/
    Card gameGrid[GRID_SIZE][GRID_SIZE];
    /** @brief The list of words.*/
    QStringList wordList;
    
    /** @brief The grid layout for the game board.*/
    QGridLayout* gridLayout;
    /** @brief The buttons representing the cards in the game grid.*/
    QPushButton* cards[GRID_SIZE][GRID_SIZE];
    
    /** @brief The labels for red team information.*/
    QLabel* redTeamLabel;
    /** @brief The label for blue team information.*/
    QLabel* blueTeamLabel;
    /** @brief The label for the current turn.*/
    QLabel* currentTurnLabel;

    /** @brief The widget for the spymaster hint*/
    SpymasterHint* spymasterHint;
    /** @brief The widget for the operator guess*/
    OperatorGuess* operatorGuess;
    /** @brief The label for the current hint*/
    QLabel* currentHint;
    /** @brief The label for the corresponding number*/
    QString correspondingNumber;

    /** @brief The transition screen widget.*/
    Transition* transition;

    /** @brief The label for red team score.*/
    QLabel* redScoreLabel;
    /** @brief The label for blue team score.*/
    QLabel* blueScoreLabel;

    /** @brief The chat box widget.*/
    ChatBox* chatBox;
    /** @brief The name of the current player.*/
    QString currentPlayerName;
    /** @brief The team of the current player.*/
    ChatBox::Team currentPlayerTeam;
    /** @brief The list of users in the game.*/
    User* users;
};

#endif // GAMEBOARD_H