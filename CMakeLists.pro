# Qt Modules
QT += core gui widgets # Add required Qt modules

CONFIG += c++17
# CONFIG += release    # Options: debug, release, console, static, shared
# CONFIG -= qtquickcompiler  # Disable Qt Quick Compiler if not needed

TARGET = Codenames
TEMPLATE = app

# Source and Header Files
SOURCES += mainwindow.cpp \
           pregame.cpp \
           main.cpp

HEADERS += pregame.h \
           mainwindow.h \
           

# FORMS += mainwindow.ui  # If using Qt Designer .ui files



