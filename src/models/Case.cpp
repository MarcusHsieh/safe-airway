#include "Case.h"
#include <QJsonDocument>
#include <QUuid>

QJsonObject SpecificationTableRow::toJson() const
{
    QJsonObject json;
    json["makeModel"] = makeModel;
    json["size"] = size;
    json["type"] = type;
    json["cuff"] = cuff;
    json["innerDiameter"] = innerDiameter;
    json["outerDiameter"] = outerDiameter;
    json["length"] = length;
    json["reorderNumber"] = reorderNumber;
    return json;
}

SpecificationTableRow SpecificationTableRow::fromJson(const QJsonObject& json)
{
    SpecificationTableRow row;
    row.makeModel = json["makeModel"].toString();
    row.size = json["size"].toString();
    row.type = json["type"].toString();
    row.cuff = json["cuff"].toString();
    row.innerDiameter = json["innerDiameter"].toDouble();
    row.outerDiameter = json["outerDiameter"].toDouble();
    row.length = json["length"].toDouble();
    row.reorderNumber = json["reorderNumber"].toInt();
    return row;
}

QJsonObject PatientInfo::toJson() const
{
    QJsonObject json;
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["mrn"] = mrn;
    json["dateOfBirth"] = dateOfBirth;
    return json;
}

PatientInfo PatientInfo::fromJson(const QJsonObject& json)
{
    PatientInfo patient;
    patient.firstName = json["firstName"].toString();
    patient.lastName = json["lastName"].toString();
    patient.mrn = json["mrn"].toString();
    patient.dateOfBirth = json["dateOfBirth"].toString();
    return patient;
}

QJsonObject SuctionInfo::toJson() const
{
    QJsonObject json;
    json["size"] = size;
    json["depth"] = depth;
    return json;
}

SuctionInfo SuctionInfo::fromJson(const QJsonObject& json)
{
    SuctionInfo suction;
    suction.size = json["size"].toInt();
    suction.depth = json["depth"].toString();
    return suction;
}

QJsonObject DecisionBox::toJson() const
{
    QJsonObject json;
    json["maskVentilate"] = maskVentilate;
    json["intubateAbove"] = intubateAbove;
    json["intubateStoma"] = intubateStoma;
    return json;
}

DecisionBox DecisionBox::fromJson(const QJsonObject& json)
{
    DecisionBox decision;
    decision.maskVentilate = json["maskVentilate"].toBool();
    decision.intubateAbove = json["intubateAbove"].toBool();
    decision.intubateStoma = json["intubateStoma"].toBool();
    return decision;
}

Case::Case()
    : id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , caseType(CaseType::Tracheostomy)
    , createdAt(QDateTime::currentDateTime())
    , updatedAt(QDateTime::currentDateTime())
{
}

Case::Case(const QString& id, CaseType type)
    : id(id)
    , caseType(type)
    , createdAt(QDateTime::currentDateTime())
    , updatedAt(QDateTime::currentDateTime())
{
}

QString Case::getCaseTypeString() const
{
    switch (caseType) {
    case CaseType::Tracheostomy:
        return "tracheostomy";
    case CaseType::NewTracheostomy:
        return "new_tracheostomy";
    case CaseType::DifficultAirway:
        return "difficult_airway";
    case CaseType::LTR:
        return "ltr";
    }
    return "tracheostomy";
}

CaseType Case::caseTypeFromString(const QString& str)
{
    if (str == "tracheostomy") return CaseType::Tracheostomy;
    if (str == "new_tracheostomy") return CaseType::NewTracheostomy;
    if (str == "difficult_airway") return CaseType::DifficultAirway;
    if (str == "ltr") return CaseType::LTR;
    return CaseType::Tracheostomy;
}

QJsonObject Case::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["caseType"] = getCaseTypeString();
    json["patient"] = patient.toJson();
    
    QJsonArray specArray;
    for (const auto& row : specTable) {
        specArray.append(row.toJson());
    }
    json["specTable"] = specArray;
    
    json["suction"] = suction.toJson();
    json["decisionBox"] = decisionBox.toJson();
    json["emergencyScenario"] = emergencyScenario;
    json["specialComments"] = specialComments;
    json["createdAt"] = createdAt.toString(Qt::ISODate);
    json["updatedAt"] = updatedAt.toString(Qt::ISODate);
    
    json["surgeon"] = surgeon;
    json["dateOfSurgery"] = dateOfSurgery;
    json["firstTrachChange"] = firstTrachChange;
    json["airwayDiagnosis"] = airwayDiagnosis;
    json["procedure"] = procedure;
    json["extubationDate"] = extubationDate;
    json["trachIndication"] = trachIndication;
    // Note: filePath is not saved to JSON to avoid circular references

    return json;
}

Case Case::fromJson(const QJsonObject& json)
{
    Case case_;
    case_.id = json["id"].toString();
    case_.caseType = caseTypeFromString(json["caseType"].toString());
    case_.patient = PatientInfo::fromJson(json["patient"].toObject());
    
    QJsonArray specArray = json["specTable"].toArray();
    for (const auto& value : specArray) {
        case_.specTable.append(SpecificationTableRow::fromJson(value.toObject()));
    }
    
    case_.suction = SuctionInfo::fromJson(json["suction"].toObject());
    case_.decisionBox = DecisionBox::fromJson(json["decisionBox"].toObject());
    case_.emergencyScenario = json["emergencyScenario"].toString();
    case_.specialComments = json["specialComments"].toString();
    case_.createdAt = QDateTime::fromString(json["createdAt"].toString(), Qt::ISODate);
    case_.updatedAt = QDateTime::fromString(json["updatedAt"].toString(), Qt::ISODate);
    
    case_.surgeon = json["surgeon"].toString();
    case_.dateOfSurgery = json["dateOfSurgery"].toString();
    case_.firstTrachChange = json["firstTrachChange"].toString();
    case_.airwayDiagnosis = json["airwayDiagnosis"].toString();
    case_.procedure = json["procedure"].toString();
    case_.extubationDate = json["extubationDate"].toString();
    case_.trachIndication = json["trachIndication"].toString();
    // Note: filePath is set by CaseManager after loading

    return case_;
}