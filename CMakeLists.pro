# Qt Modules
QT += core gui widgets # Add required Qt modules

# Set C++ Standard
CONFIG += c++17

# Enable Warnings
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic -Werror

# Build Type (Uncomment one)
CONFIG += debug
# CONFIG += release    # Options: debug, release, console, static, shared
# CONFIG -= qtquickcompiler  # Disable Qt Quick Compiler if not needed

TARGET = Codenames
TEMPLATE = app

# Source and Header Files
SOURCES += src/main.cpp

# HEADERS += src/

# FORMS += src/mainwindow.ui  # If using Qt Designer .ui files

# Output Directory
DESTDIR = $$PWD/bin
