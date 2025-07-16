#ifndef BASEFORMWIDGET_H
#define BASEFORMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextOption>
#include <QPushButton>
#include <QTableWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QScrollArea>
#include <QTimer>
#include "models/Case.h"
#include "widgets/PatientInfoWidget.h"
#include "widgets/EmergencyPanel.h"
#include "widgets/TextWrapDelegate.h"

class BaseFormWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseFormWidget(CaseType caseType, QWidget* parent = nullptr);
    
    void finishSetup(); // Call this after construction to complete form-specific setup
    
    virtual void setCase(const Case& case_);
    virtual Case getCase() const;
    
    void setFrozen(bool frozen);
    bool isFrozen() const { return frozen_; }
    
    virtual bool isFormValid() const;
    virtual QStringList getValidationErrors() const;
    
    void clear();
    
    CaseType getCaseType() const { return caseType_; }
    
    bool wasJustSaved() const { return justSaved_; }
    void setJustSaved(bool saved) { justSaved_ = saved; }

signals:
    void saveRequested();
    void freezeModeToggled(bool frozen);
    void formChanged();
    void emergencyScenarioChanged(const QString& scenario);
    void backRequested();

protected slots:
    void onSaveClicked();
    void onFreezeClicked();
    void onPrintClicked();
    void onBackClicked();
    void onPatientInfoChanged();
    void onEmergencyScenarioSelected(const QString& scenario);
    void onSuctionSizeChanged(int size);
    void onSpecTableChanged();
    void onFormFieldChanged();

protected:
    virtual void setupUI();
    virtual void setupSpecificationTable();
    virtual void setupDecisionBoxes();
    virtual void setupSidePanel();
    virtual void setupActionButtons();
    virtual void setupFormSpecificFields() {}
    
    virtual void updateStyles();
    virtual void updateEmergencyAdvice();
    
    void addSpecificationRow();
    void removeSpecificationRow();
    void moveSpecificationRowUp();
    void moveSpecificationRowDown();
    
    CaseType caseType_;
    Case currentCase_;
    bool frozen_;
    bool justSaved_;
    
    QScrollArea* scrollArea_;
    QWidget* contentWidget_;
    QVBoxLayout* mainLayout_;
    QVBoxLayout* contentLayout_;
    QVBoxLayout* rightLayout_;
    QVBoxLayout* formFieldsLayout_;
    
    QLabel* headerLabel_;
    QLabel* logoLabel_;
    PatientInfoWidget* patientInfoWidget_;
    
    QTableWidget* specificationTable_;
    QTextEdit* makeModelEdit_;
    QPushButton* addRowButton_;
    QPushButton* removeRowButton_;
    QPushButton* moveUpButton_;
    QPushButton* moveDownButton_;
    
    EmergencyPanel* emergencyPanel_;
    
    QGroupBox* decisionBoxGroup_;
    QCheckBox* maskVentilateCheckBox_;
    QCheckBox* intubateAboveCheckBox_;
    QCheckBox* intubateStomaCheckBox_;
    
    QGroupBox* sidePanelGroup_;
    QSpinBox* suctionSizeSpinBox_;
    QLineEdit* suctionDepthEdit_;
    QTextEdit* specialCommentsEdit_;
    
    QPushButton* freezeButton_;
    QPushButton* saveButton_;
    QPushButton* printButton_;
    QPushButton* backButton_;
    
    void connectSignals();
    void loadFormData();
    void saveFormData();
    void updateTableFromCase();
    void updateCaseFromTable();
    
private:
    void setupHeader();
    void setupSpecTableButtons();
    void printForm();
};

#endif // BASEFORMWIDGET_H