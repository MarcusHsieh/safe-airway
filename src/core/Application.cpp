#include "Application.h"
#include "CaseManager.h"
#include "utils/ConfigManager.h"
#include "utils/StyleManager.h"
#include <QStandardPaths>
#include <QDir>

Application* Application::instance_ = nullptr;

Application::Application()
    : app_(nullptr)
    , caseManager_(nullptr)
{
}

Application::~Application()
{
    shutdown();
}

Application& Application::instance()
{
    if (!instance_) {
        instance_ = new Application();
    }
    return *instance_;
}

bool Application::initialize(QApplication* app)
{
    app_ = app;
    
    if (!app_) {
        return false;
    }
    
    app_->setApplicationName("Safe Airway");
    app_->setApplicationVersion("1.0.0");
    app_->setOrganizationName("Nemours Children's Health");
    app_->setOrganizationDomain("nemours.org");
    
    initializeServices();
    setupStyles();
    
    return true;
}

void Application::shutdown()
{
    if (caseManager_) {
        delete caseManager_;
        caseManager_ = nullptr;
    }
}

void Application::initializeServices()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString safePath = documentsPath + "/SafeAirway";
    QDir().mkpath(safePath);
    
    caseManager_ = new CaseManager(this);
    caseManager_->initialize(safePath);
    
    ConfigManager::instance().saveFontSize(StyleManager::instance().getBaseFontSize());
}

void Application::setupStyles()
{
    int savedFontSize = ConfigManager::instance().getFontSize();
    if (savedFontSize > 0) {
        StyleManager::instance().setBaseFontSize(savedFontSize);
    }
}