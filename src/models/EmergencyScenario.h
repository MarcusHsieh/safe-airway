#ifndef EMERGENCYSCENARIO_H
#define EMERGENCYSCENARIO_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QList>

class EmergencyScenario
{
public:
    EmergencyScenario();
    EmergencyScenario(const QString& name, const QStringList& instructions);
    
    QString getName() const { return name; }
    void setName(const QString& scenarioName) { name = scenarioName; }
    
    QStringList getInstructions() const { return instructions; }
    void setInstructions(const QStringList& instructionList) { instructions = instructionList; }
    
    QString getFormattedInstructions() const;
    
    QJsonObject toJson() const;
    static EmergencyScenario fromJson(const QJsonObject& json);
    
    static QList<EmergencyScenario> getDefaultScenarios();
    static EmergencyScenario getScenarioByName(const QString& name);
    
private:
    QString name;
    QStringList instructions;
};

#endif // EMERGENCYSCENARIO_H