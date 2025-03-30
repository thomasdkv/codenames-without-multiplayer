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
 */
class SpymasterHint : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the SpymasterHint class.
   * @param parent The parent widget.
   */
  explicit SpymasterHint(QWidget* parent = nullptr);

  /**
   * @brief Destructor for the SpymasterHint class.
   */
  ~SpymasterHint();

  /**
   * @brief Resets the spymaster hint input fields.
   */
  void reset();

signals:
  /**
   * @brief Signal emitted when a hint is submitted.
   * @param hint The hint text.
   * @param number The number of words associated with the hint.
   */
  void hintSubmitted(const QString& hint, const int number);

private slots:
  /**
   * @brief Slot to handle the submission of a hint.
   */
  void submitHint();

  /**
   * @brief Slot to update the button's clickable state based on input.
   */
  void updateButtonClickable();

  /**
   * @brief Slot to convert text to uppercase.
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
