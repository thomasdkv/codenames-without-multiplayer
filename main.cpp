#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    // Create the Qt application object
    QApplication app(argc, argv);

    // Create the main window
    QMainWindow window;

    // Set the window title
    window.setWindowTitle("My Qt Window");

    // Show the window
    window.show();

    // Start the application event loop
    return app.exec();
}