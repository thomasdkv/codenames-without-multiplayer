# Qt Modules
QT += core gui widgets

CONFIG += c++17
# CONFIG += release    # Options: debug, release, console, static, shared
# CONFIG -= qtquickcompiler  # Disable Qt Quick Compiler if not needed

TARGET = Codenames
TEMPLATE = app

# Source and Header Files
SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/pregame.cpp \
           src/localstart.cpp

HEADERS += include/mainwindow.h \
           include/pregame.h \
           include/localstart.h

# Output Directory
DESTDIR = $$PWD/bin

# Object Directory
OBJECTS_DIR = $$PWD/build

INCLUDEPATH += $$PWD/include