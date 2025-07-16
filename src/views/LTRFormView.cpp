#include "LTRFormView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

LTRFormView::LTRFormView(QWidget* parent)
    : BaseFormWidget(CaseType::LTR, parent)
    , additionalFieldsGroup_(nullptr)
    , procedureEdit_(nullptr)
    , surgeonEdit_(nullptr)
    , dateOfSurgeryEdit_(nullptr)
    , extubationDateEdit_(nullptr)
{
}

void LTRFormView::setupFormSpecificFields()
{
    additionalFieldsGroup_ = new QGroupBox("LTR Information");
    QVBoxLayout* layout = new QVBoxLayout(additionalFieldsGroup_);
    
    QHBoxLayout* procedureLayout = new QHBoxLayout();
    procedureLayout->addWidget(new QLabel("Procedure:"));
    procedureEdit_ = new QLineEdit();
    procedureEdit_->setPlaceholderText("Enter procedure details");
    procedureLayout->addWidget(procedureEdit_);
    
    QHBoxLayout* surgeonLayout = new QHBoxLayout();
    surgeonLayout->addWidget(new QLabel("Surgeon:"));
    surgeonEdit_ = new QLineEdit();
    surgeonEdit_->setPlaceholderText("Enter surgeon name");
    surgeonLayout->addWidget(surgeonEdit_);
    
    QHBoxLayout* dateOfSurgeryLayout = new QHBoxLayout();
    dateOfSurgeryLayout->addWidget(new QLabel("Date of Surgery:"));
    dateOfSurgeryEdit_ = new QLineEdit();
    dateOfSurgeryEdit_->setPlaceholderText("MM/DD/YYYY");
    dateOfSurgeryLayout->addWidget(dateOfSurgeryEdit_);
    
    QHBoxLayout* extubationLayout = new QHBoxLayout();
    extubationLayout->addWidget(new QLabel("Extubation Date:"));
    extubationDateEdit_ = new QLineEdit();
    extubationDateEdit_->setPlaceholderText("MM/DD/YYYY");
    extubationLayout->addWidget(extubationDateEdit_);
    
    layout->addLayout(procedureLayout);
    layout->addLayout(surgeonLayout);
    layout->addLayout(dateOfSurgeryLayout);
    layout->addLayout(extubationLayout);
    
    // Add the group box to the form fields layout in BaseFormWidget
    if (formFieldsLayout_) {
        formFieldsLayout_->addWidget(additionalFieldsGroup_);
    }
    
    connectAdditionalSignals();
}

void LTRFormView::connectAdditionalSignals()
{
    connect(procedureEdit_, &QLineEdit::textChanged, this, &LTRFormView::onFormFieldChanged);
    connect(surgeonEdit_, &QLineEdit::textChanged, this, &LTRFormView::onFormFieldChanged);
    connect(dateOfSurgeryEdit_, &QLineEdit::textChanged, this, &LTRFormView::onFormFieldChanged);
    connect(extubationDateEdit_, &QLineEdit::textChanged, this, &LTRFormView::onFormFieldChanged);
}

void LTRFormView::setCase(const Case& case_)
{
    BaseFormWidget::setCase(case_);
    
    if (procedureEdit_) procedureEdit_->setText(case_.getProcedure());
    if (surgeonEdit_) surgeonEdit_->setText(case_.getSurgeon());
    if (dateOfSurgeryEdit_) dateOfSurgeryEdit_->setText(case_.getDateOfSurgery());
    if (extubationDateEdit_) extubationDateEdit_->setText(case_.getExtubationDate());
}

Case LTRFormView::getCase() const
{
    Case case_ = BaseFormWidget::getCase();
    
    if (procedureEdit_) case_.setProcedure(procedureEdit_->text());
    if (surgeonEdit_) case_.setSurgeon(surgeonEdit_->text());
    if (dateOfSurgeryEdit_) case_.setDateOfSurgery(dateOfSurgeryEdit_->text());
    if (extubationDateEdit_) case_.setExtubationDate(extubationDateEdit_->text());
    
    return case_;
}