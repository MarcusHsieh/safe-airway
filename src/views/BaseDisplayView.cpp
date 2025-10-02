#include "BaseDisplayView.h"
#include "utils/StyleManager.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>

BaseDisplayView::BaseDisplayView(CaseType caseType, QWidget* parent)
    : QWidget(parent)
    , caseType_(caseType)
    , mainLayout_(nullptr)
    , headerLayout_(nullptr)
    , contentLayout_(nullptr)
    , logoLabel_(nullptr)
    , titleLabel_(nullptr)
    , patientGroup_(nullptr)
    , patientNameLabel_(nullptr)
    , trachGroup_(nullptr)
    , trachDetailLabel_(nullptr)
    , emergencyGroup_(nullptr)
    , maskVentilateLabel_(nullptr)
    , intubateAboveLabel_(nullptr)
    , intubateStomaLabel_(nullptr)
    , caseInfoGroup_(nullptr)
    , surgeonLabel_(nullptr)
    , dateLabel_(nullptr)
    , emergencyContactLabel_(nullptr)
    , buttonLayout_(nullptr)
    , backToFormButton_(nullptr)
    , closeDisplayButton_(nullptr)
    , specificContentWidget_(nullptr)
    , specificContentLayout_(nullptr)
{
    setupUI();
    updateStyles();
}

void BaseDisplayView::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setSpacing(20);
    mainLayout_->setContentsMargins(30, 30, 30, 30);

    // Get case type color
    QString caseColor = getCaseTypeColor();

    // Set background color based on case type
    setStyleSheet(QString("QWidget#BaseDisplayView { background-color: %1; }").arg(caseColor));
    setObjectName("BaseDisplayView");

    // Header section with logo and title
    headerLayout_ = new QHBoxLayout();
    headerLayout_->setSpacing(20);

    logoLabel_ = new QLabel();
    logoLabel_->setAlignment(Qt::AlignCenter);
    logoLabel_->setMaximumHeight(100);
    logoLabel_->setMaximumWidth(300);

    // Load logo
    QPixmap logo(":/images/nemours-logo-full.png");
    if (!logo.isNull()) {
        logoLabel_->setPixmap(logo.scaled(300, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel_->setText("Nemours Children's Health");
        logoLabel_->setStyleSheet("font-weight: bold; color: #0066CC; font-size: 24px;");
    }

    titleLabel_ = new QLabel("Safe Airway - Display Mode");
    titleLabel_->setAlignment(Qt::AlignCenter);

    headerLayout_->addWidget(logoLabel_);
    headerLayout_->addWidget(titleLabel_, 1);

    mainLayout_->addLayout(headerLayout_);

    // Main content area
    contentLayout_ = new QVBoxLayout();
    contentLayout_->setSpacing(30);

    setupPatientInfo();
    setupTrachInfo();
    setupEmergencyInfo();

    // Case-specific content area
    specificContentWidget_ = new QWidget();
    specificContentLayout_ = new QVBoxLayout(specificContentWidget_);
    setupCaseSpecificFields();

    contentLayout_->addWidget(patientGroup_);
    contentLayout_->addWidget(trachGroup_);
    contentLayout_->addWidget(emergencyGroup_);
    contentLayout_->addWidget(specificContentWidget_);

    mainLayout_->addLayout(contentLayout_);

    // Bottom control buttons (small and discrete)
    buttonLayout_ = new QHBoxLayout();
    buttonLayout_->setAlignment(Qt::AlignRight);

    backToFormButton_ = new QPushButton("Back to Form");
    closeDisplayButton_ = new QPushButton("Close Display");

    buttonLayout_->addWidget(backToFormButton_);
    buttonLayout_->addWidget(closeDisplayButton_);

    mainLayout_->addLayout(buttonLayout_);

    connect(backToFormButton_, &QPushButton::clicked, this, &BaseDisplayView::onBackToFormClicked);
    connect(closeDisplayButton_, &QPushButton::clicked, this, &BaseDisplayView::onCloseDisplayClicked);
}

void BaseDisplayView::setupPatientInfo()
{
    patientGroup_ = new QGroupBox("Patient Information");

    QVBoxLayout* layout = new QVBoxLayout(patientGroup_);
    layout->setAlignment(Qt::AlignCenter);

    patientNameLabel_ = new QLabel();
    patientNameLabel_->setAlignment(Qt::AlignCenter);
    patientNameLabel_->setStyleSheet("font-size: 72px; font-weight: bold; color: #333; margin: 20px;");

    layout->addWidget(patientNameLabel_);
}

void BaseDisplayView::setupTrachInfo()
{
    trachGroup_ = new QGroupBox("Tracheostomy Information");

    QVBoxLayout* layout = new QVBoxLayout(trachGroup_);
    layout->setAlignment(Qt::AlignCenter);

    trachDetailLabel_ = new QLabel();
    trachDetailLabel_->setAlignment(Qt::AlignCenter);
    trachDetailLabel_->setStyleSheet("font-size: 48px; font-weight: bold; color: #000; margin: 15px;");
    trachDetailLabel_->setWordWrap(true);

    layout->addWidget(trachDetailLabel_);
}

void BaseDisplayView::setupEmergencyInfo()
{
    emergencyGroup_ = new QGroupBox("Emergency Procedures");
    emergencyGroup_->setStyleSheet("QGroupBox { border: 3px solid #FF0000; background-color: #FFEEEE; }");

    QVBoxLayout* layout = new QVBoxLayout(emergencyGroup_);
    layout->setSpacing(15);

    maskVentilateLabel_ = new QLabel("1. Mask Ventilate");
    maskVentilateLabel_->setStyleSheet("font-size: 36px; font-weight: bold; color: #FF0000; padding: 10px;");

    intubateAboveLabel_ = new QLabel("2. Intubate from Above");
    intubateAboveLabel_->setStyleSheet("font-size: 36px; font-weight: bold; color: #FF0000; padding: 10px;");

    intubateStomaLabel_ = new QLabel("3. Intubate through Stoma");
    intubateStomaLabel_->setStyleSheet("font-size: 36px; font-weight: bold; color: #FF0000; padding: 10px;");

    layout->addWidget(maskVentilateLabel_);
    layout->addWidget(intubateAboveLabel_);
    layout->addWidget(intubateStomaLabel_);

    // Emergency contact
    emergencyContactLabel_ = new QLabel("In Case of Emergency Call: 5-5555");
    emergencyContactLabel_->setAlignment(Qt::AlignCenter);
    emergencyContactLabel_->setStyleSheet("font-size: 28px; font-weight: bold; color: #FF0000; background-color: #FFFF00; padding: 15px; border: 2px solid #FF0000;");

    layout->addWidget(emergencyContactLabel_);
}

void BaseDisplayView::setCase(const Case& case_)
{
    currentCase_ = case_;
    updateDisplay();
}

void BaseDisplayView::updateDisplay()
{
    PatientInfo patient = currentCase_.getPatient();

    // Only show first name for HIPAA compliance (no DOB, MRN, or last name)
    patientNameLabel_->setText(patient.firstName);

    // Update trach information based on specification table
    QList<SpecificationTableRow> specs = currentCase_.getSpecTable();
    if (!specs.isEmpty()) {
        SpecificationTableRow spec = specs.first();
        QString trachDetail = QString("%1 %2 %3")
                              .arg(spec.type)
                              .arg(spec.size)
                              .arg(spec.cuff);
        trachDetailLabel_->setText(trachDetail);

        // Update ETT size recommendations in emergency procedures
        intubateAboveLabel_->setText(QString("2. Intubate from Above with %1 ETT").arg(spec.size));
        intubateStomaLabel_->setText(QString("3. Intubate through Stoma with %1 ETT").arg(spec.size));
    }

    // Update decision box status
    DecisionBox decision = currentCase_.getDecisionBox();

    maskVentilateLabel_->setVisible(decision.maskVentilate);
    intubateAboveLabel_->setVisible(decision.intubateAbove);
    intubateStomaLabel_->setVisible(decision.intubateStoma);
}

QString BaseDisplayView::getCaseTypeColor() const
{
    switch (caseType_) {
        case CaseType::Tracheostomy:
            return "#e8e8e8";
        case CaseType::NewTracheostomy:
            return "#ff66cc";
        case CaseType::DifficultAirway:
            return "#ffc000";
        case CaseType::LTR:
            return "#00b0f0";
        default:
            return "#FFFFFF";
    }
}

void BaseDisplayView::updateStyles()
{
    titleLabel_->setFont(StyleManager::instance().getHeaderFont());
    patientGroup_->setFont(StyleManager::instance().getBodyFont());
    trachGroup_->setFont(StyleManager::instance().getBodyFont());
    emergencyGroup_->setFont(StyleManager::instance().getBodyFont());

    backToFormButton_->setFont(StyleManager::instance().getBodyFont());
    closeDisplayButton_->setFont(StyleManager::instance().getBodyFont());
}

void BaseDisplayView::onBackToFormClicked()
{
    emit backToFormRequested();
}

void BaseDisplayView::onCloseDisplayClicked()
{
    emit closeDisplayRequested();
}