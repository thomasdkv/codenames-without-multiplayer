/**
 * @file spymasterhint.h
 * @author Matthew Marbina (Group 9)
 * @brief Header file for the SpymasterHint class, which provides a UI for the spymaster to give hints.
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef SPYMASTERHINT_H
#define SPYMASTERHINT_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpressionValidator>

/**
 * @class SpymasterHint
 * @brief A widget for the spymaster to input a hint and the number of words associated with it.
 * @details This class contains a QLineEdit for the hint, a QSpinBox for the number of words, and a QPushButton 
 * to submit the hint. It also includes validation to ensure the hint is a single word and updates the button's 
 * clickable state based on input.
 * @author Group 9
 */
class SpymasterHint : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the SpymasterHint class.
   * @details This constructor sets up the layout and initializes the widgets. 
   * It connects the button to the submitHint slot and the LineEdit to the updateButtonClickable slot.
   * It also sets up a validator to ensure the hint is a single word and connects the textChanged signal to
   * the textToUppercase slot to convert the hint to uppercase.
   * @param parent The parent widget.
   */
  explicit SpymasterHint(QWidget* parent = nullptr);

  /**
   * @brief Destructor for the SpymasterHint class.
   * @details This destructor cleans up the resources used by the class.
   * It does not need to explicitly delete the widgets as they are managed by Qt's parent-child system.
   */
  ~SpymasterHint();

  /**
   * @brief Resets the spymaster hint input fields.
   * @details This function clears the hint input field and resets the number of words to 1.
   * It also updates the button's clickable state to ensure it is disabled until valid input is provided.
   */
  void reset();

signals:
  /**
   * @brief Signal emitted when a hint is submitted.
   * @details This signal is emitted when the spymaster submits a hint and the number of words.
   * It carries the hint text and the number of words as parameters.
   * @param hint The hint text.
   * @param number The number of words associated with the hint.
   */
  void hintSubmitted(const QString& hint, const int number);

private slots:
  /**
   * @brief Slot to handle the submission of a hint.
   * @details This function retrieves the hint and number of words from the input fields,
   * emits the hintSubmitted signal, and resets the input fields.
   */
  void submitHint();

  /**
   * @brief Slot to update the button's clickable state based on input.
   * @details This function checks if the hint input field is empty.
   * If the input is valid, it enables the button; otherwise, it disables it.
   */
  void updateButtonClickable();

  /**
   * @brief Slot to convert text to uppercase.
   * @details This function is called when the text in the hint input field changes.
   * It converts the text to uppercase to ensure consistency in the hint format.
   * @param text The input text.
   */
  void textToUppercase(const QString& text);

private:
  /**
   * @brief QLineEdit used by the spymaster to input the hint.
   */
  QLineEdit* hintLineEdit; 

  /**
   * @brief QSpinBox used by the spymaster to input the number of words correlated to the hint.
   */
  QSpinBox* numberSpinBox; 

  /**
   * @brief QPushButton to submit the hint.
   */
  QPushButton* giveClueButton; 

  /**
   * @brief QRegularExpressionValidator used to validate the hint the spymaster inputs is a single valid word.
   */
  QRegularExpressionValidator* textValidator; 
};

#endif // SPYMASTERHINT_H
