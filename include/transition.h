#ifndef TRANSITION_H
#define TRANSITION_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class Transition : public QWidget {
    Q_OBJECT

public:
    explicit Transition(QWidget* parent = nullptr);
    ~Transition();
    void setMessage(const QString& message);

signals:
    void continueClicked();

private:
    QLabel* messageLabel;
    QPushButton* continueButton;
};

#endif // TRANSITION_H