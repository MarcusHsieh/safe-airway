#include "EmergencyScenario.h"
#include <QJsonArray>

EmergencyScenario::EmergencyScenario()
{
}

EmergencyScenario::EmergencyScenario(const QString& name, const QStringList& instructions)
    : name(name), instructions(instructions)
{
}

QString EmergencyScenario::getFormattedInstructions() const
{
    QString formatted;
    for (int i = 0; i < instructions.size(); ++i) {
        formatted += QString("%1. %2\n").arg(i + 1).arg(instructions[i]);
    }
    return formatted.trimmed();
}

QJsonObject EmergencyScenario::toJson() const
{
    QJsonObject json;
    json["name"] = name;
    
    QJsonArray instructionArray;
    for (const QString& instruction : instructions) {
        instructionArray.append(instruction);
    }
    json["instructions"] = instructionArray;
    
    return json;
}

EmergencyScenario EmergencyScenario::fromJson(const QJsonObject& json)
{
    EmergencyScenario scenario;
    scenario.name = json["name"].toString();
    
    QJsonArray instructionArray = json["instructions"].toArray();
    for (const auto& value : instructionArray) {
        scenario.instructions.append(value.toString());
    }
    
    return scenario;
}

QList<EmergencyScenario> EmergencyScenario::getDefaultScenarios()
{
    QList<EmergencyScenario> scenarios;
    
    scenarios.append(EmergencyScenario("can't suction", {
        "Call for help immediately",
        "Position patient appropriately",
        "Attempt gentle suctioning with smaller catheter",
        "Consider saline instillation",
        "If unsuccessful, prepare for emergency intervention"
    }));
    
    scenarios.append(EmergencyScenario("can't ventilate", {
        "Call for help immediately",
        "Check tracheostomy tube patency",
        "Attempt bag-mask ventilation above stoma",
        "Consider 6.0 endotracheal tube insertion",
        "Prepare for emergency surgical intervention"
    }));
    
    scenarios.append(EmergencyScenario("o2 sat drop", {
        "Increase oxygen concentration",
        "Check tube position and patency",
        "Suction if needed",
        "Assess for pneumothorax",
        "Call for respiratory support"
    }));
    
    scenarios.append(EmergencyScenario("decannulation", {
        "Stay calm and call for help",
        "Cover stoma with gauze",
        "Attempt reinsertion with same size tube",
        "If unsuccessful, try smaller tube",
        "Consider 6.0 endotracheal tube if needed",
        "Prepare for emergency surgical intervention"
    }));
    
    scenarios.append(EmergencyScenario("hemoptysis", {
        "Position patient to protect airway",
        "Suction blood from tracheostomy",
        "Apply pressure if bleeding from stoma",
        "Call for emergency assistance",
        "Prepare for bronchoscopy"
    }));
    
    return scenarios;
}

EmergencyScenario EmergencyScenario::getScenarioByName(const QString& name)
{
    auto scenarios = getDefaultScenarios();
    for (const auto& scenario : scenarios) {
        if (scenario.getName() == name) {
            return scenario;
        }
    }
    return EmergencyScenario();
}