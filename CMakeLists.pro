# Qt Modules
QT += core gui widgets # Add required Qt modules

CONFIG += c++17
# CONFIG += release    # Options: debug, release, console, static, shared
# CONFIG -= qtquickcompiler  # Disable Qt Quick Compiler if not needed

TARGET = Codenames
TEMPLATE = app

# Source and Header Files
SOURCES += src/mainwindow.cpp \
           src/pregame.cpp \
           src/main.cpp

HEADERS += include/pregame.h \
           include/mainwindow.h \ 

# Output Directory
DESTDIR = $$PWD/bin

# Object Directory
OBJECTS_DIR = $$PWD/build

INCLUDEPATH += $$PWD/include

RESOURCES += src/resources.qrc


