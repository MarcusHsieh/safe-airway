#ifndef CASEMANAGER_H
#define CASEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QFileSystemWatcher>
#include "models/Case.h"

class CaseManager : public QObject
{
    Q_OBJECT

public:
    explicit CaseManager(QObject* parent = nullptr);
    
    bool initialize(const QString& basePath);
    
    QString saveCase(const Case& case_);
    bool loadCase(const QString& filePath, Case& case_);
    bool deleteCase(const QString& filePath);
    
    QStringList getRecentCases() const;
    QStringList getCasesByType(CaseType caseType) const;
    QStringList getAllCases() const;
    
    bool exportCase(const Case& case_, const QString& filePath);
    bool importCase(const QString& filePath, Case& case_);
    
    void setAutoSaveEnabled(bool enabled);
    bool isAutoSaveEnabled() const { return autoSaveEnabled_; }
    
    void setAutoSaveInterval(int minutes);
    int getAutoSaveInterval() const { return autoSaveInterval_; }
    
    QString getBasePath() const { return basePath_; }
    QString getCaseDirectory(CaseType caseType) const;
    
signals:
    void caseSaved(const QString& filePath);
    void caseLoaded(const QString& filePath);
    void caseDeleted(const QString& filePath);
    void casesChanged();
    void autoSaveCompleted(const QString& filePath);
    void error(const QString& message);

public slots:
    void onAutoSaveTimer();
    
private:
    QString basePath_;
    bool autoSaveEnabled_;
    int autoSaveInterval_;
    QTimer* autoSaveTimer_;
    QFileSystemWatcher* fileWatcher_;
    
    void createDirectoryStructure();
    QString generateCaseFilename(const Case& case_) const;
    QString getCaseTypeDirectory(CaseType caseType) const;
    
    void updateRecentCases(const QString& filePath);
    
private slots:
    void onDirectoryChanged(const QString& path);
    void onFileChanged(const QString& path);
};

#endif // CASEMANAGER_H