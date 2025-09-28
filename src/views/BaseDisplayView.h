#ifndef BASEDISPLAYVIEW_H
#define BASEDISPLAYVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QGroupBox>
#include "models/Case.h"

class BaseDisplayView : public QWidget
{
    Q_OBJECT

public:
    explicit BaseDisplayView(CaseType caseType, QWidget* parent = nullptr);
    virtual ~BaseDisplayView() = default;

    virtual void setCase(const Case& case_);
    Case getCase() const { return currentCase_; }

signals:
    void backToFormRequested();
    void closeDisplayRequested();

protected:
    virtual void setupUI();
    virtual void setupPatientInfo();
    virtual void setupTrachInfo();
    virtual void setupEmergencyInfo();
    virtual void setupCaseSpecificFields() = 0;
    virtual void updateDisplay();

    void updateStyles();

protected slots:
    void onBackToFormClicked();
    void onCloseDisplayClicked();

protected:
    CaseType caseType_;
    Case currentCase_;

    // UI Components
    QVBoxLayout* mainLayout_;
    QHBoxLayout* headerLayout_;
    QVBoxLayout* contentLayout_;

    // Header section
    QLabel* logoLabel_;
    QLabel* titleLabel_;

    // Patient information (large, visible)
    QGroupBox* patientGroup_;
    QLabel* patientNameLabel_;

    // Trach tube information (large display)
    QGroupBox* trachGroup_;
    QLabel* trachDetailLabel_;

    // Emergency procedures (large, bold)
    QGroupBox* emergencyGroup_;
    QLabel* maskVentilateLabel_;
    QLabel* intubateAboveLabel_;
    QLabel* intubateStomaLabel_;

    // Additional case info
    QGroupBox* caseInfoGroup_;
    QLabel* surgeonLabel_;
    QLabel* dateLabel_;
    QLabel* emergencyContactLabel_;

    // Control buttons
    QHBoxLayout* buttonLayout_;
    QPushButton* backToFormButton_;
    QPushButton* closeDisplayButton_;

    // Case-specific content area
    QWidget* specificContentWidget_;
    QVBoxLayout* specificContentLayout_;
};

#endif // BASEDISPLAYVIEW_H