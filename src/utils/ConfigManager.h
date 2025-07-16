#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QSettings>
#include <QSize>
#include <QPoint>

class ConfigManager
{
public:
    static ConfigManager& instance();
    
    void saveWindowGeometry(const QString& windowName, const QSize& size, const QPoint& position);
    void loadWindowGeometry(const QString& windowName, QSize& size, QPoint& position);
    
    void saveLastDirectory(const QString& directory);
    QString getLastDirectory() const;
    
    void saveRecentCases(const QStringList& cases);
    QStringList getRecentCases() const;
    
    void saveUserPreference(const QString& key, const QVariant& value);
    QVariant getUserPreference(const QString& key, const QVariant& defaultValue = QVariant()) const;
    
    void saveFontSize(int fontSize);
    int getFontSize() const;
    
    void saveAutoSaveInterval(int minutes);
    int getAutoSaveInterval() const;
    
private:
    ConfigManager();
    ~ConfigManager();
    
    QSettings* settings;
    static ConfigManager* instance_;
};

#endif // CONFIGMANAGER_H