#ifndef CASE_H
#define CASE_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QList>

enum class CaseType {
    Tracheostomy,
    NewTracheostomy,
    DifficultAirway,
    LTR
};

struct SpecificationTableRow {
    QString makeModel;
    QString size;
    QString type;
    QString cuff;
    double innerDiameter;
    double outerDiameter;
    double length;
    int reorderNumber;
    
    QJsonObject toJson() const;
    static SpecificationTableRow fromJson(const QJsonObject& json);
};

struct PatientInfo {
    QString firstName;
    QString lastName;
    QString mrn;
    QString dateOfBirth;
    
    QJsonObject toJson() const;
    static PatientInfo fromJson(const QJsonObject& json);
};

struct SuctionInfo {
    int size;
    QString depth;
    
    QJsonObject toJson() const;
    static SuctionInfo fromJson(const QJsonObject& json);
};

struct DecisionBox {
    bool maskVentilate;
    bool intubateAbove;
    bool intubateStoma;
    
    QJsonObject toJson() const;
    static DecisionBox fromJson(const QJsonObject& json);
};

class Case
{
public:
    Case();
    Case(const QString& id, CaseType type);
    
    QString getId() const { return id; }
    void setId(const QString& newId) { id = newId; }
    
    CaseType getCaseType() const { return caseType; }
    void setCaseType(CaseType type) { caseType = type; }
    
    QString getCaseTypeString() const;
    static CaseType caseTypeFromString(const QString& str);
    
    PatientInfo getPatient() const { return patient; }
    void setPatient(const PatientInfo& patientInfo) { patient = patientInfo; }
    
    QList<SpecificationTableRow> getSpecTable() const { return specTable; }
    void setSpecTable(const QList<SpecificationTableRow>& table) { specTable = table; }
    
    SuctionInfo getSuction() const { return suction; }
    void setSuction(const SuctionInfo& suctionInfo) { suction = suctionInfo; }
    
    DecisionBox getDecisionBox() const { return decisionBox; }
    void setDecisionBox(const DecisionBox& decision) { decisionBox = decision; }
    
    QString getEmergencyScenario() const { return emergencyScenario; }
    void setEmergencyScenario(const QString& scenario) { emergencyScenario = scenario; }
    
    QString getSpecialComments() const { return specialComments; }
    void setSpecialComments(const QString& comments) { specialComments = comments; }
    
    QDateTime getCreatedAt() const { return createdAt; }
    QDateTime getUpdatedAt() const { return updatedAt; }
    void setUpdatedAt(const QDateTime& time) { updatedAt = time; }
    
    QString getSurgeon() const { return surgeon; }
    void setSurgeon(const QString& surgeonName) { surgeon = surgeonName; }
    
    QString getDateOfSurgery() const { return dateOfSurgery; }
    void setDateOfSurgery(const QString& date) { dateOfSurgery = date; }
    
    QString getFirstTrachChange() const { return firstTrachChange; }
    void setFirstTrachChange(const QString& date) { firstTrachChange = date; }
    
    QString getAirwayDiagnosis() const { return airwayDiagnosis; }
    void setAirwayDiagnosis(const QString& diagnosis) { airwayDiagnosis = diagnosis; }
    
    QString getProcedure() const { return procedure; }
    void setProcedure(const QString& procedureName) { procedure = procedureName; }
    
    QString getExtubationDate() const { return extubationDate; }
    void setExtubationDate(const QString& date) { extubationDate = date; }
    
    QString getTrachIndication() const { return trachIndication; }
    void setTrachIndication(const QString& indication) { trachIndication = indication; }

    QString getFilePath() const { return filePath; }
    void setFilePath(const QString& path) { filePath = path; }

    QJsonObject toJson() const;
    static Case fromJson(const QJsonObject& json);
    
private:
    QString id;
    CaseType caseType;
    PatientInfo patient;
    QList<SpecificationTableRow> specTable;
    SuctionInfo suction;
    DecisionBox decisionBox;
    QString emergencyScenario;
    QString specialComments;
    QDateTime createdAt;
    QDateTime updatedAt;
    
    QString surgeon;
    QString dateOfSurgery;
    QString firstTrachChange;
    QString airwayDiagnosis;
    QString procedure;
    QString extubationDate;
    QString trachIndication;
    QString filePath;
};

#endif // CASE_H