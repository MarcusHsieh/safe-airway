#include "PatientInfoWidget.h"
#include "utils/StyleManager.h"

PatientInfoWidget::PatientInfoWidget(QWidget* parent)
    : QWidget(parent)
    , firstNameEdit_(nullptr)
    , lastNameEdit_(nullptr)
    , mrnEdit_(nullptr)
    , dobEdit_(nullptr)
    , firstNameLabel_(nullptr)
    , lastNameLabel_(nullptr)
    , mrnLabel_(nullptr)
    , dobLabel_(nullptr)
    , frozen_(false)
{
    setupUI();
    connectSignals();
    updateStyles();
}

void PatientInfoWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(15);
    firstNameLabel_ = new QLabel("First:");
    firstNameLabel_->setMinimumWidth(100);
    firstNameEdit_ = new QLineEdit();
    firstNameEdit_->setMinimumHeight(30);
    firstNameEdit_->setPlaceholderText("Patient's first name");
    nameLayout->addWidget(firstNameLabel_);
    nameLayout->addWidget(firstNameEdit_);
    
    lastNameLabel_ = new QLabel("Last:");
    lastNameLabel_->setMinimumWidth(100);
    lastNameEdit_ = new QLineEdit();
    lastNameEdit_->setMinimumHeight(30);
    lastNameEdit_->setPlaceholderText("Patient's last name");
    nameLayout->addWidget(lastNameLabel_);
    nameLayout->addWidget(lastNameEdit_);
    
    QHBoxLayout* infoLayout = new QHBoxLayout();
    infoLayout->setSpacing(15);
    mrnLabel_ = new QLabel("MRN:");
    mrnLabel_->setMinimumWidth(100);
    mrnEdit_ = new QLineEdit();
    mrnEdit_->setMinimumHeight(30);
    mrnEdit_->setPlaceholderText("Medical Record #");
    infoLayout->addWidget(mrnLabel_);
    infoLayout->addWidget(mrnEdit_);
    
    dobLabel_ = new QLabel("DoB:");
    dobLabel_->setMinimumWidth(100);
    dobEdit_ = new QLineEdit();
    dobEdit_->setMinimumHeight(30);
    dobEdit_->setPlaceholderText("MM/DD/YYYY");
    infoLayout->addWidget(dobLabel_);
    infoLayout->addWidget(dobEdit_);
    
    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(infoLayout);
    
    setLayout(mainLayout);
}

void PatientInfoWidget::connectSignals()
{
    connect(firstNameEdit_, &QLineEdit::textChanged, this, &PatientInfoWidget::onFieldChanged);
    connect(lastNameEdit_, &QLineEdit::textChanged, this, &PatientInfoWidget::onFieldChanged);
    connect(mrnEdit_, &QLineEdit::textChanged, this, &PatientInfoWidget::onFieldChanged);
    connect(dobEdit_, &QLineEdit::textChanged, this, &PatientInfoWidget::onFieldChanged);
}

void PatientInfoWidget::updateStyles()
{
    StyleManager& styleManager = StyleManager::instance();
    
    QFont bodyFont = styleManager.getBodyFont();
    firstNameLabel_->setFont(bodyFont);
    lastNameLabel_->setFont(bodyFont);
    mrnLabel_->setFont(bodyFont);
    dobLabel_->setFont(bodyFont);
    
    firstNameEdit_->setFont(bodyFont);
    lastNameEdit_->setFont(bodyFont);
    mrnEdit_->setFont(bodyFont);
    dobEdit_->setFont(bodyFont);
}

void PatientInfoWidget::setPatientInfo(const PatientInfo& patient)
{
    firstNameEdit_->setText(patient.firstName);
    lastNameEdit_->setText(patient.lastName);
    mrnEdit_->setText(patient.mrn);
    dobEdit_->setText(patient.dateOfBirth);
}

PatientInfo PatientInfoWidget::getPatientInfo() const
{
    PatientInfo patient;
    patient.firstName = firstNameEdit_->text();
    patient.lastName = lastNameEdit_->text();
    patient.mrn = mrnEdit_->text();
    patient.dateOfBirth = dobEdit_->text();
    return patient;
}

void PatientInfoWidget::setFrozen(bool frozen)
{
    frozen_ = frozen;
    
    firstNameEdit_->setReadOnly(frozen);
    lastNameEdit_->setReadOnly(frozen);
    mrnEdit_->setReadOnly(frozen);
    dobEdit_->setReadOnly(frozen);
    
    if (frozen) {
        StyleManager::instance().applyFreezeOverlay(this, true);
    } else {
        StyleManager::instance().applyFreezeOverlay(this, false);
        updateStyles();
    }
}

void PatientInfoWidget::clear()
{
    firstNameEdit_->clear();
    lastNameEdit_->clear();
    mrnEdit_->clear();
    dobEdit_->clear();
}

void PatientInfoWidget::onFieldChanged()
{
    emit patientInfoChanged();
}