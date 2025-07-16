#include "NewTracheostomyFormView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

NewTracheostomyFormView::NewTracheostomyFormView(QWidget* parent)
    : BaseFormWidget(CaseType::NewTracheostomy, parent)
    , additionalFieldsGroup_(nullptr)
    , trachIndicationEdit_(nullptr)
    , surgeonEdit_(nullptr)
    , dateOfSurgeryEdit_(nullptr)
    , firstTrachChangeEdit_(nullptr)
{
}

void NewTracheostomyFormView::setupFormSpecificFields()
{
    additionalFieldsGroup_ = new QGroupBox("New Tracheostomy Information");
    QVBoxLayout* layout = new QVBoxLayout(additionalFieldsGroup_);
    
    QHBoxLayout* indicationLayout = new QHBoxLayout();
    indicationLayout->addWidget(new QLabel("Trach Indication:"));
    trachIndicationEdit_ = new QLineEdit();
    trachIndicationEdit_->setPlaceholderText("Enter tracheostomy indication");
    indicationLayout->addWidget(trachIndicationEdit_);
    
    QHBoxLayout* surgeonLayout = new QHBoxLayout();
    surgeonLayout->addWidget(new QLabel("Surgeon:"));
    surgeonEdit_ = new QLineEdit();
    surgeonEdit_->setPlaceholderText("Enter surgeon name");
    surgeonLayout->addWidget(surgeonEdit_);
    
    QHBoxLayout* dateLayout = new QHBoxLayout();
    dateLayout->addWidget(new QLabel("Date of Surgery:"));
    dateOfSurgeryEdit_ = new QLineEdit();
    dateOfSurgeryEdit_->setPlaceholderText("MM/DD/YYYY");
    dateLayout->addWidget(dateOfSurgeryEdit_);
    
    QHBoxLayout* changeLayout = new QHBoxLayout();
    changeLayout->addWidget(new QLabel("First Trach Change:"));
    firstTrachChangeEdit_ = new QLineEdit();
    firstTrachChangeEdit_->setPlaceholderText("MM/DD/YYYY");
    changeLayout->addWidget(firstTrachChangeEdit_);
    
    layout->addLayout(indicationLayout);
    layout->addLayout(surgeonLayout);
    layout->addLayout(dateLayout);
    layout->addLayout(changeLayout);
    
    // Add the group box to the form fields layout in BaseFormWidget
    if (formFieldsLayout_) {
        formFieldsLayout_->addWidget(additionalFieldsGroup_);
    }
    
    connectAdditionalSignals();
}

void NewTracheostomyFormView::connectAdditionalSignals()
{
    connect(trachIndicationEdit_, &QLineEdit::textChanged, this, &NewTracheostomyFormView::onFormFieldChanged);
    connect(surgeonEdit_, &QLineEdit::textChanged, this, &NewTracheostomyFormView::onFormFieldChanged);
    connect(dateOfSurgeryEdit_, &QLineEdit::textChanged, this, &NewTracheostomyFormView::onFormFieldChanged);
    connect(firstTrachChangeEdit_, &QLineEdit::textChanged, this, &NewTracheostomyFormView::onFormFieldChanged);
}

void NewTracheostomyFormView::setCase(const Case& case_)
{
    BaseFormWidget::setCase(case_);
    
    if (trachIndicationEdit_) trachIndicationEdit_->setText(case_.getTrachIndication());
    if (surgeonEdit_) surgeonEdit_->setText(case_.getSurgeon());
    if (dateOfSurgeryEdit_) dateOfSurgeryEdit_->setText(case_.getDateOfSurgery());
    if (firstTrachChangeEdit_) firstTrachChangeEdit_->setText(case_.getFirstTrachChange());
}

Case NewTracheostomyFormView::getCase() const
{
    Case case_ = BaseFormWidget::getCase();
    
    if (trachIndicationEdit_) case_.setTrachIndication(trachIndicationEdit_->text());
    if (surgeonEdit_) case_.setSurgeon(surgeonEdit_->text());
    if (dateOfSurgeryEdit_) case_.setDateOfSurgery(dateOfSurgeryEdit_->text());
    if (firstTrachChangeEdit_) case_.setFirstTrachChange(firstTrachChangeEdit_->text());
    
    return case_;
}