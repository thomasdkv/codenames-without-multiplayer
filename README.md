# Codenames Game

Codenames is a multiplayer word association game where two teams compete to find all their agents' codenames first. This implementation uses Qt for the user interface and WebSockets for real-time interaction between players.

## Requirements

To build and run this application, you will need the following dependencies:

- Qt Core
- Qt GUI
- Qt Widgets
- Qt WebSockets

To install the required dependencies, use the following command:

```bash
sudo apt install qtbase5-dev qtwebsockets5-dev
```

## Building and Running the Project

Clone the repository or download the project files. In the project directory, run the following commands to build the application:

```bash
qmake
make clean
make
```

After successfully building the project, run the application based on your operating system:
- On Linux, use the following command:
  ```bash
  ./bin/Codenames
  ```



## Features
- Real-time gameplay with WebSockets for seamless multiplayer experience.
- Real-time local gameplay
- Intuitive graphical interface built using Qt's GUI and widgets.
- Support for multiple platforms (Linux/macOS).
- A fun and challenging game where players guess the correct codenames based on clues.