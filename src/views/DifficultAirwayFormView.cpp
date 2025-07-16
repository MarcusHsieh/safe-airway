#include "DifficultAirwayFormView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

DifficultAirwayFormView::DifficultAirwayFormView(QWidget* parent)
    : BaseFormWidget(CaseType::DifficultAirway, parent)
    , additionalFieldsGroup_(nullptr)
    , airwayDiagnosisEdit_(nullptr)
{
}

void DifficultAirwayFormView::setupFormSpecificFields()
{
    additionalFieldsGroup_ = new QGroupBox("Airway Information");
    QVBoxLayout* layout = new QVBoxLayout(additionalFieldsGroup_);
    
    QHBoxLayout* diagnosisLayout = new QHBoxLayout();
    diagnosisLayout->addWidget(new QLabel("Airway Diagnosis:"));
    airwayDiagnosisEdit_ = new QLineEdit();
    airwayDiagnosisEdit_->setPlaceholderText("Enter airway diagnosis");
    diagnosisLayout->addWidget(airwayDiagnosisEdit_);
    
    layout->addLayout(diagnosisLayout);
    
    // Add the group box to the form fields layout in BaseFormWidget
    if (formFieldsLayout_) {
        formFieldsLayout_->addWidget(additionalFieldsGroup_);
    }
    
    connectAdditionalSignals();
}

void DifficultAirwayFormView::connectAdditionalSignals()
{
    connect(airwayDiagnosisEdit_, &QLineEdit::textChanged, this, &DifficultAirwayFormView::onFormFieldChanged);
}

void DifficultAirwayFormView::setCase(const Case& case_)
{
    BaseFormWidget::setCase(case_);
    
    if (airwayDiagnosisEdit_) airwayDiagnosisEdit_->setText(case_.getAirwayDiagnosis());
}

Case DifficultAirwayFormView::getCase() const
{
    Case case_ = BaseFormWidget::getCase();
    
    if (airwayDiagnosisEdit_) case_.setAirwayDiagnosis(airwayDiagnosisEdit_->text());
    
    return case_;
}