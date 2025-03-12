#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class PreGame;  // Forward declaration of PreGame class

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openPreGame();
    void showMainWindow();

private:
    QWidget *centralWidget;
    QVBoxLayout *layout;

    QLabel *titleLabel;

    QPushButton *localPlayButton;
    QPushButton *onlinePlayButton;
    QPushButton *statsButton;

    PreGame *preGameWindow;  // PreGame pointer to the second window
};

#endif // MAINWINDOW_H