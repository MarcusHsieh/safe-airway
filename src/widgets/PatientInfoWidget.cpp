#include "PatientInfoWidget.h"
#include "utils/StyleManager.h"
#include <QGuiApplication>
#include <QScreen>

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
    
    // Use percentage-based spacing and margins
    QSize screenSize = QGuiApplication::primaryScreen()->availableSize();
    int spacing = screenSize.height() * 0.01; // 1% of screen height
    int margin = screenSize.width() * 0.01; // 1% of screen width
    
    mainLayout->setSpacing(spacing);
    mainLayout->setContentsMargins(margin, margin, margin, margin);
    
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(15);
    firstNameLabel_ = new QLabel("Name:");
    // Use percentage-based sizing for patient name field
    int labelWidth = screenSize.width() * 0.08; // 8% of screen width
    int fieldHeight = screenSize.height() * 0.05; // 5% of screen height
    firstNameLabel_->setMinimumWidth(labelWidth);
    firstNameEdit_ = new QLineEdit();
    firstNameEdit_->setMinimumHeight(fieldHeight);
    firstNameEdit_->setPlaceholderText("First name");
    nameLayout->addWidget(firstNameLabel_);
    nameLayout->addWidget(firstNameEdit_);
    
    // Hide last name from UI but keep in backend for data persistence
    lastNameLabel_ = new QLabel("Last:");
    lastNameLabel_->setMinimumWidth(100);
    lastNameEdit_ = new QLineEdit();
    lastNameEdit_->setMinimumHeight(30);
    lastNameEdit_->setPlaceholderText("Patient's last name");
    // Don't add to layout - keep hidden for HIPAA compliance
    // nameLayout->addWidget(lastNameLabel_);
    // nameLayout->addWidget(lastNameEdit_);
    
    // Hide MRN from UI but keep in backend for data persistence
    mrnLabel_ = new QLabel("MRN:");
    mrnLabel_->setMinimumWidth(100);
    mrnEdit_ = new QLineEdit();
    mrnEdit_->setMinimumHeight(30);
    mrnEdit_->setPlaceholderText("Medical Record #");
    // Don't add to layout - keep hidden for HIPAA compliance
    
    // Hide DOB from UI but keep in backend for data persistence
    dobLabel_ = new QLabel("Date of Birth:");
    dobLabel_->setMinimumWidth(labelWidth);
    dobEdit_ = new QLineEdit();
    dobEdit_->setMinimumHeight(fieldHeight);
    dobEdit_->setPlaceholderText("MM/DD/YYYY");
    // Don't add to layout - keep hidden to save screen space on small displays
    
    mainLayout->addLayout(nameLayout);
    
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
    QFont patientNameFont = styleManager.getPatientNameFont();
    
    // Patient name should be extra large for visibility
    firstNameLabel_->setFont(patientNameFont);
    
    // Make the input text even larger using stylesheet to override form styles
    firstNameEdit_->setStyleSheet(
        "QLineEdit {"
        "   font-size: 60px;"
        "   background-color: #FAFAFA;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #1976D2;"
        "   background-color: white;"
        "}"
    );
    
    // Other fields use regular font (but they're hidden anyway)
    lastNameLabel_->setFont(bodyFont);
    lastNameEdit_->setFont(bodyFont);
    mrnLabel_->setFont(bodyFont);
    mrnEdit_->setFont(bodyFont);
    dobLabel_->setFont(bodyFont);
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