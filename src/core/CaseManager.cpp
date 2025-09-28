#include "CaseManager.h"
#include "utils/ConfigManager.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QStandardPaths>

CaseManager::CaseManager(QObject* parent)
    : QObject(parent)
    , autoSaveEnabled_(true)
    , autoSaveInterval_(5)
    , autoSaveTimer_(nullptr)
    , fileWatcher_(nullptr)
{
    autoSaveTimer_ = new QTimer(this);
    autoSaveTimer_->setSingleShot(false);
    connect(autoSaveTimer_, &QTimer::timeout, this, &CaseManager::onAutoSaveTimer);
    
    fileWatcher_ = new QFileSystemWatcher(this);
    connect(fileWatcher_, &QFileSystemWatcher::directoryChanged, this, &CaseManager::onDirectoryChanged);
    connect(fileWatcher_, &QFileSystemWatcher::fileChanged, this, &CaseManager::onFileChanged);
}

bool CaseManager::initialize(const QString& basePath)
{
    basePath_ = basePath;
    
    createDirectoryStructure();
    
    QDir baseDir(basePath_);
    if (!baseDir.exists()) {
        emit error("Failed to create base directory: " + basePath_);
        return false;
    }
    
    fileWatcher_->addPath(basePath_);
    
    if (autoSaveEnabled_) {
        autoSaveTimer_->start(autoSaveInterval_ * 60 * 1000);
    }
    
    return true;
}

void CaseManager::createDirectoryStructure()
{
    QDir baseDir(basePath_);
    if (!baseDir.exists()) {
        baseDir.mkpath(".");
    }
    
    QDir caseSavesDir(basePath_ + "/case_saves");
    if (!caseSavesDir.exists()) {
        caseSavesDir.mkpath(".");
    }
    
    QStringList caseTypes = {"tracheostomy", "new_tracheostomy", "difficult_airway", "ltr"};
    for (const QString& type : caseTypes) {
        QDir typeDir(basePath_ + "/case_saves/" + type);
        if (!typeDir.exists()) {
            typeDir.mkpath(".");
        }
        fileWatcher_->addPath(typeDir.absolutePath());
    }
}

QString CaseManager::saveCase(const Case& case_)
{
    QString filePath;

    // Check if this is an existing case with a known file path
    QString existingPath = case_.getFilePath();
    if (!existingPath.isEmpty() && QFile::exists(existingPath)) {
        // Use existing file path to overwrite
        filePath = existingPath;
    } else {
        // Generate new filename for new cases
        QString filename = generateCaseFilename(case_);
        QString typeDir = getCaseTypeDirectory(case_.getCaseType());
        filePath = typeDir + "/" + filename;
    }

    QJsonDocument doc(case_.toJson());

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error("Failed to save case: " + file.errorString());
        return QString();
    }

    file.write(doc.toJson());
    file.close();

    updateRecentCases(filePath);

    emit caseSaved(filePath);
    return filePath;
}

bool CaseManager::loadCase(const QString& filePath, Case& case_)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error("Failed to load case: " + file.errorString());
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        emit error("Failed to parse case file: " + filePath);
        return false;
    }
    
    case_ = Case::fromJson(doc.object());
    case_.setFilePath(filePath);  // Set the file path after loading

    updateRecentCases(filePath);

    emit caseLoaded(filePath);
    return true;
}

bool CaseManager::deleteCase(const QString& filePath)
{
    QFile file(filePath);
    if (!file.remove()) {
        emit error("Failed to delete case: " + file.errorString());
        return false;
    }
    
    emit caseDeleted(filePath);
    return true;
}

QStringList CaseManager::getRecentCases() const
{
    return ConfigManager::instance().getRecentCases();
}

QStringList CaseManager::getCasesByType(CaseType caseType) const
{
    QString typeDir = getCaseTypeDirectory(caseType);
    QDir dir(typeDir);
    
    QStringList filters;
    filters << "*.json";
    
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Time);
    
    QStringList fullPaths;
    for (const QString& file : files) {
        fullPaths << dir.absoluteFilePath(file);
    }
    
    return fullPaths;
}

QStringList CaseManager::getAllCases() const
{
    QStringList allCases;
    
    QStringList caseTypes = {"tracheostomy", "new_tracheostomy", "difficult_airway", "ltr"};
    for (const QString& type : caseTypes) {
        CaseType caseType = Case::caseTypeFromString(type);
        allCases << getCasesByType(caseType);
    }
    
    return allCases;
}

bool CaseManager::exportCase(const Case& case_, const QString& filePath)
{
    QJsonDocument doc(case_.toJson());
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error("Failed to export case: " + file.errorString());
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    return true;
}

bool CaseManager::importCase(const QString& filePath, Case& case_)
{
    return loadCase(filePath, case_);
}

void CaseManager::setAutoSaveEnabled(bool enabled)
{
    autoSaveEnabled_ = enabled;
    
    if (enabled) {
        autoSaveTimer_->start(autoSaveInterval_ * 60 * 1000);
    } else {
        autoSaveTimer_->stop();
    }
}

void CaseManager::setAutoSaveInterval(int minutes)
{
    autoSaveInterval_ = minutes;
    
    if (autoSaveEnabled_) {
        autoSaveTimer_->start(autoSaveInterval_ * 60 * 1000);
    }
}

QString CaseManager::getCaseDirectory(CaseType caseType) const
{
    return getCaseTypeDirectory(caseType);
}

QString CaseManager::generateCaseFilename(const Case& case_) const
{
    // Use only the case ID for consistent filenames (no timestamp)
    // This ensures the same case always has the same filename
    QString id = case_.getId();
    return QString("%1.json").arg(id);
}

QString CaseManager::getCaseTypeDirectory(CaseType caseType) const
{
    QString typeString;
    switch (caseType) {
    case CaseType::Tracheostomy:
        typeString = "tracheostomy";
        break;
    case CaseType::NewTracheostomy:
        typeString = "new_tracheostomy";
        break;
    case CaseType::DifficultAirway:
        typeString = "difficult_airway";
        break;
    case CaseType::LTR:
        typeString = "ltr";
        break;
    }
    
    return basePath_ + "/case_saves/" + typeString;
}

void CaseManager::updateRecentCases(const QString& filePath)
{
    QStringList recentCases = getRecentCases();
    
    recentCases.removeAll(filePath);
    
    recentCases.prepend(filePath);
    
    while (recentCases.size() > 10) {
        recentCases.removeLast();
    }
    
    ConfigManager::instance().saveRecentCases(recentCases);
}

void CaseManager::onAutoSaveTimer()
{
    emit autoSaveCompleted(QString());
}

void CaseManager::onDirectoryChanged(const QString& path)
{
    Q_UNUSED(path)
    emit casesChanged();
}

void CaseManager::onFileChanged(const QString& path)
{
    Q_UNUSED(path)
    emit casesChanged();
}