#include "TracheostomyFormView.h"
#include "utils/StyleManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QScreen>

TracheostomyFormView::TracheostomyFormView(QWidget* parent)
    : BaseFormWidget(CaseType::Tracheostomy, parent)
    , specificFieldsGroup_(nullptr)
    , trachIndicationEdit_(nullptr)
    , trachIndicationLabel_(nullptr)
{
}

void TracheostomyFormView::setupFormSpecificFields()
{
    specificFieldsGroup_ = new QGroupBox("Tracheostomy Information");
    specificFieldsGroup_->setFont(StyleManager::instance().getGroupBoxFont());
    QVBoxLayout* groupLayout = new QVBoxLayout(specificFieldsGroup_);
    
    QHBoxLayout* indicationLayout = new QHBoxLayout();
    trachIndicationLabel_ = new QLabel("Trach Indication:");
    // Use percentage-based sizing for trach indication field
    QSize screenSize = QGuiApplication::primaryScreen()->availableSize();
    int labelWidth = screenSize.width() * 0.10; // 10% of screen width
    int fieldHeight = screenSize.height() * 0.04; // 4% of screen height
    
    trachIndicationLabel_->setMinimumWidth(labelWidth);
    trachIndicationLabel_->setFont(StyleManager::instance().getFormLabelFont());
    
    trachIndicationEdit_ = new QLineEdit();
    trachIndicationEdit_->setPlaceholderText("Enter tracheostomy indication");
    trachIndicationEdit_->setMinimumHeight(fieldHeight);
    trachIndicationEdit_->setFont(StyleManager::instance().getFormInputFont());
    
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