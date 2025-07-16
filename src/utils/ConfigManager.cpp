#include "ConfigManager.h"
#include <QStandardPaths>
#include <QDir>

ConfigManager* ConfigManager::instance_ = nullptr;

ConfigManager::ConfigManager()
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configPath);
    settings = new QSettings(configPath + "/settings.ini", QSettings::IniFormat);
}

ConfigManager::~ConfigManager()
{
    delete settings;
}

ConfigManager& ConfigManager::instance()
{
    if (!instance_) {
        instance_ = new ConfigManager();
    }
    return *instance_;
}

void ConfigManager::saveWindowGeometry(const QString& windowName, const QSize& size, const QPoint& position)
{
    settings->beginGroup("WindowGeometry");
    settings->setValue(windowName + "_size", size);
    settings->setValue(windowName + "_position", position);
    settings->endGroup();
    settings->sync();
}

void ConfigManager::loadWindowGeometry(const QString& windowName, QSize& size, QPoint& position)
{
    settings->beginGroup("WindowGeometry");
    size = settings->value(windowName + "_size", QSize(1024, 768)).toSize();
    position = settings->value(windowName + "_position", QPoint(100, 100)).toPoint();
    settings->endGroup();
}

void ConfigManager::saveLastDirectory(const QString& directory)
{
    settings->setValue("LastDirectory", directory);
    settings->sync();
}

QString ConfigManager::getLastDirectory() const
{
    return settings->value("LastDirectory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
}

void ConfigManager::saveRecentCases(const QStringList& cases)
{
    settings->setValue("RecentCases", cases);
    settings->sync();
}

QStringList ConfigManager::getRecentCases() const
{
    return settings->value("RecentCases", QStringList()).toStringList();
}

void ConfigManager::saveUserPreference(const QString& key, const QVariant& value)
{
    settings->beginGroup("UserPreferences");
    settings->setValue(key, value);
    settings->endGroup();
    settings->sync();
}

QVariant ConfigManager::getUserPreference(const QString& key, const QVariant& defaultValue) const
{
    settings->beginGroup("UserPreferences");
    QVariant value = settings->value(key, defaultValue);
    settings->endGroup();
    return value;
}

void ConfigManager::saveFontSize(int fontSize)
{
    saveUserPreference("FontSize", fontSize);
}

int ConfigManager::getFontSize() const
{
    return getUserPreference("FontSize", 12).toInt();
}

void ConfigManager::saveAutoSaveInterval(int minutes)
{
    saveUserPreference("AutoSaveInterval", minutes);
}

int ConfigManager::getAutoSaveInterval() const
{
    return getUserPreference("AutoSaveInterval", 5).toInt();
}