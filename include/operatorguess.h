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

    void reset();

signals:
    void guessSubmitted();

private slots:
    void submitGuess();

private:
    QPushButton* submitGuessButton;
};

#endif // OPERATORGUESS_H