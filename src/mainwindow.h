#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
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
    QPushButton *pushButton;
    PreGame *preGameWindow;  // PreGame pointer to the second window
};

#endif // MAINWINDOW_H