#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QIcon>
#include "core/Application.h"
#include "views/MainWindow.h"

int main(int argc, char *argv[])
{
    // Set environment variable to disable DPI scaling in Qt 6 on Windows
    // This prevents Qt from applying Windows display scaling on top of our explicit pixel sizes
    // Our UI uses 32px fonts designed for visibility, and should not be further scaled
    // Only apply on Windows to avoid Linux issues
#ifdef Q_OS_WIN
    qputenv("QT_SCALE_FACTOR", "1.0");
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
#endif

    QApplication app(argc, argv);

    // Set application icon
    app.setWindowIcon(QIcon(":/images/nemours-logo.png"));

    if (!Application::instance().initialize(&app)) {
        QMessageBox::critical(nullptr, "Initialization Error", 
                            "Failed to initialize the application. Please check your installation.");
        return -1;
    }
    
    MainWindow window;
    window.show();
    
    int result = app.exec();
    
    Application::instance().shutdown();
    
    return result;
}