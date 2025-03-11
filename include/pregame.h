#ifndef PREGAME_H
#define PREGAME_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class PreGame : public QWidget {
    Q_OBJECT

public:
    explicit PreGame(QWidget *parent = nullptr);
    ~PreGame();

private slots:
    void goBackToMain();  // Slot to handle back button click

signals:
    void backToMainWindow();  // Signal to notify MainWindow to show itself

private:
    QLabel *label;
    QPushButton *backButton;
    QVBoxLayout *layout;
};

#endif // PREGAME_H