#ifndef OPERATORGUESS_H
#define OPERATORGUESS_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpressionValidator>

class OperatorGuess : public QWidget {
    Q_OBJECT

public:
    explicit OperatorGuess(QWidget* parent = nullptr);
    ~OperatorGuess();

signals:
    void guessSubmitted();

private slots:
    void submitGuess();
    void updateButtonClickable();
    void textToUppercase(const QString& text);

private:
    QLineEdit* guessLineEdit;
    QPushButton* submitGuessButton;
    QRegularExpressionValidator* textValidator;
};

#endif // OPERATORGUESS_H