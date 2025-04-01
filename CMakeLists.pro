# Qt Modules
QT += core gui widgets
# QT += websockets

CONFIG += c++17
# CONFIG += release    # Options: debug, release, console, static, shared
# CONFIG -= qtquickcompiler  # Disable Qt Quick Compiler if not needed

TARGET = Codenames
TEMPLATE = app


SOURCES += $$files($$PWD/src/*.cpp, true)
# SOURCES += $$files($$PWD/src/Multiplayer/*.cpp, true)
HEADERS += $$files($$PWD/include/*.h, true)
# HEADERS += $$files($$PWD/include/Multiplayer/*.h, true)

# Remove duplicates if necessary (qmake uses += so the files won't be added twice as long as it's done correctly).
SOURCES = $$unique(SOURCES)
HEADERS = $$unique(HEADERS)


# Output Directory
DESTDIR = $$PWD/bin

# Object Directory
OBJECTS_DIR = $$PWD/build

INCLUDEPATH += $$PWD/include

RESOURCES += $$PWD/resources.qrc