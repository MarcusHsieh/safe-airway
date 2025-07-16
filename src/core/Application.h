#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>

class CaseManager;
class ConfigManager;
class StyleManager;

class Application : public QObject
{
    Q_OBJECT

public:
    static Application& instance();
    
    bool initialize(QApplication* app);
    void shutdown();
    
    CaseManager* getCaseManager() const { return caseManager_; }
    
private:
    Application();
    ~Application();
    
    static Application* instance_;
    
    QApplication* app_;
    CaseManager* caseManager_;
    
    void initializeServices();
    void setupStyles();
};

#endif // APPLICATION_H