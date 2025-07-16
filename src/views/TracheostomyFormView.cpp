#include "TracheostomyFormView.h"
#include "utils/StyleManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

TracheostomyFormView::TracheostomyFormView(QWidget* parent)
    : BaseFormWidget(CaseType::Tracheostomy, parent)
    , specificFieldsGroup_(nullptr)
    , trachIndicationEdit_(nullptr)
    , trachIndicationLabel_(nullptr)
{
}

void TracheostomyFormView::setupFormSpecificFields()
{
    specificFieldsGroup_ = new QGroupBox("Tracheostomy Specific Information");
    QVBoxLayout* groupLayout = new QVBoxLayout(specificFieldsGroup_);
    
    QHBoxLayout* indicationLayout = new QHBoxLayout();
    trachIndicationLabel_ = new QLabel("Trach Indication:");
    trachIndicationLabel_->setMinimumWidth(120);
    trachIndicationEdit_ = new QLineEdit();
    trachIndicationEdit_->setPlaceholderText("Enter tracheostomy indication");
    
    indicationLayout->addWidget(trachIndicationLabel_);
    indicationLayout->addWidget(trachIndicationEdit_);
    
    groupLayout->addLayout(indicationLayout);
    
    // Add the group box to the form fields layout in BaseFormWidget
    if (formFieldsLayout_) {
        formFieldsLayout_->addWidget(specificFieldsGroup_);
    }
    
    // Connect signal for form changes
    connect(trachIndicationEdit_, &QLineEdit::textChanged, this, &TracheostomyFormView::onFormFieldChanged);
}

void TracheostomyFormView::setCase(const Case& case_)
{
    BaseFormWidget::setCase(case_);
    
    if (trachIndicationEdit_) {
        trachIndicationEdit_->setText(case_.getTrachIndication());
    }
}

Case TracheostomyFormView::getCase() const
{
    Case case_ = BaseFormWidget::getCase();
    
    if (trachIndicationEdit_) {
        case_.setTrachIndication(trachIndicationEdit_->text());
    }
    
    return case_;
}