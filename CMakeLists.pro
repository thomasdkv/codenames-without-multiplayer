# Qt Modules
QT += core gui widgets websockets

CONFIG += c++17
# CONFIG += release    # Options: debug, release, console, static, shared
# CONFIG -= qtquickcompiler  # Disable Qt Quick Compiler if not needed

TARGET = Codenames
TEMPLATE = app

# Source and Header Files
SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/pregame.cpp \
           src/gameboard.cpp \ 
           src/spymasterhint.cpp \
           src/operatorguess.cpp \
           src/player.cpp \
           src/game.cpp \
           src/transition.cpp \
           src/Multiplayer/multimain.cpp \
           src/Multiplayer/multipregame.cpp \
           src/Multiplayer/multiboard.cpp

HEADERS += include/mainwindow.h \
           include/pregame.h \
           include/gameboard.h \
           include/spymasterhint.h \
           include/operatorguess.h \
           include/role.h \
           include/team.h \
           include/player.h \
           include/game.h \
           include/transition.h \
           include/Multiplayer/multimain.h \
           include/Multiplayer/multipregame.h \
            include/Multiplayer/multiboard.h


# Output Directory
DESTDIR = $$PWD/bin

# Object Directory
OBJECTS_DIR = $$PWD/build

INCLUDEPATH += $$PWD/include

RESOURCES += resources.qrc