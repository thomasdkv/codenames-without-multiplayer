/**
 * @file tutorial.h
 * @brief Declaration of the Tutorial class.
 */

#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>

/**
 * @class Tutorial
 * @brief The tutorial window that guides users through the game mechanics.
 */
class Tutorial : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for Tutorial.
   * @param parent The parent widget (default is nullptr).
   */
  explicit Tutorial(QWidget* parent = nullptr);

  /**
   * @brief Destructor for Tutorial.
   */
  ~Tutorial();

 signals:
  /**
   * @brief Signal emitted when the tutorial is closed.
   */
  void tutorialClosed();

 protected:
  /**
   * @brief Handles the close event.
   * @param event Pointer to the close event.
   */
  void closeEvent(QCloseEvent* event) override;

 private slots:
  /**
   * @brief Handles the continue button click event.
   */
  void onContinueClicked();

 private:
  /**
   * @brief Updates the position of the continue button.
   */
  void updateContinueButtonPosition();

  /**
   * @brief Resets the tutorial to its initial state.
   */
  void resetTutorial();

  QWidget* centralWidget;       ///< Pointer to the central widget.
  QLabel* titleLabel;           ///< Label for the tutorial title.
  QLabel* textBox;              ///< Label for displaying tutorial text.
  QPushButton* continueButton;  ///< Button for continuing through the tutorial.
  int clickCount;               ///< Counter for continue button clicks.
};

#endif  // TUTORIAL_H
