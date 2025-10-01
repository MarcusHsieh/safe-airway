#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include "core/Application.h"
#include "views/MainWindow.h"

int main(int argc, char *argv[])
{
    // Enable high DPI scaling for Windows (Qt 6)
    // This ensures the application respects Windows display scaling settings
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    
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