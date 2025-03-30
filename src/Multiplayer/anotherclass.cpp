#include "anotherclass.h"
#include <QScreen>
#include <QGuiApplication>

AnotherClass::AnotherClass(QWidget* parent) : QWidget(parent) {
    this->setFixedSize(1000, 800); // Set fixed size for AnotherClass

    // Center the window on the screen
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

    // Initialize layout and other UI elements
} 