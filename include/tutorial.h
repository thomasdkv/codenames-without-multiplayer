#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QCloseEvent>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

class Tutorial : public QMainWindow {
    Q_OBJECT

public:
    explicit Tutorial(QWidget *parent = nullptr);
    ~Tutorial();

signals:
    void tutorialClosed();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onContinueClicked();  // New slot for continue button

private:
    void updateContinueButtonPosition();  // New function to update button position
    void resetTutorial();  // Add this new function
    QWidget* centralWidget;
    QLabel* titleLabel;
    QLabel* textBox;
    QPushButton* continueButton;
    int clickCount;  // Counter for continue clicks
};

#endif // TUTORIAL_H
