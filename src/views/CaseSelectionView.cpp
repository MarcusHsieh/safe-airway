#include "CaseSelectionView.h"
#include "utils/StyleManager.h"
#include "core/Application.h"
#include "core/CaseManager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDateTime>
#include <QShowEvent>
#include <QPixmap>

CaseSelectionView::CaseSelectionView(QWidget* parent)
    : QWidget(parent)
    , mainLayout_(nullptr)
    , titleLabel_(nullptr)
    , logoLabel_(nullptr)
    , newCaseGroup_(nullptr)
    , tracheostomyButton_(nullptr)
    , newTracheostomyButton_(nullptr)
    , difficultAirwayButton_(nullptr)
    , ltrButton_(nullptr)
    , existingCaseGroup_(nullptr)
    , recentCasesList_(nullptr)
    , loadCaseButton_(nullptr)
    , refreshButton_(nullptr)
    , selectedCaseType_(CaseType::Tracheostomy)
{
    setupUI();
    loadRecentCases();
    updateStyles();
}

void CaseSelectionView::setupUI()
{
    // Set background color for the entire view
    setStyleSheet("QWidget#CaseSelectionView { background-color: #ECEFF1; }");
    setObjectName("CaseSelectionView");

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setSpacing(25);
    mainLayout_->setContentsMargins(40, 30, 40, 30);

    // Header section with logo and title
    QWidget* headerWidget = new QWidget();
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setSpacing(15);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    // Logo
    logoLabel_ = new QLabel();
    logoLabel_->setAlignment(Qt::AlignCenter);
    logoLabel_->setMaximumHeight(100);

    QPixmap logo(":/images/nemours-logo-full.png");
    if (!logo.isNull()) {
        logoLabel_->setPixmap(logo.scaled(400, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel_->setText("Nemours Children's Health");
        logoLabel_->setStyleSheet("font-weight: bold; color: #0066CC; font-size: 32px;");
    }
    headerLayout->addWidget(logoLabel_);

    // Title
    titleLabel_ = new QLabel("Safe Airway - Case Selection");
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setFont(StyleManager::instance().getHeaderFont());
    titleLabel_->setStyleSheet("font-size: 32px; font-weight: bold; color: #2C3E50; margin-top: 10px;");
    headerLayout->addWidget(titleLabel_);

    mainLayout_->addWidget(headerWidget);

    // Content section with cards
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(25);

    setupNewCaseSection();
    setupExistingCaseSection();

    contentLayout->addWidget(newCaseGroup_, 40);
    contentLayout->addWidget(existingCaseGroup_, 60);

    mainLayout_->addLayout(contentLayout);
    mainLayout_->addStretch();

    setLayout(mainLayout_);
}

void CaseSelectionView::setupNewCaseSection()
{
    newCaseGroup_ = new QGroupBox("Create New Case");
    newCaseGroup_->setStyleSheet(
        "QGroupBox {"
        "   background-color: white;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 12px;"
        "   padding: 20px;"
        "   margin-top: 15px;"
        "   font-size: 32px;"
        "   font-weight: bold;"
        "   color: #2C3E50;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   subcontrol-position: top left;"
        "   padding: 5px 10px;"
        "   background-color: white;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(newCaseGroup_);
    layout->setSpacing(12);
    layout->setContentsMargins(15, 25, 15, 15);

    QLabel* instructionLabel = new QLabel("Select the type of case you want to create:");
    instructionLabel->setFont(StyleManager::instance().getBodyFont());
    instructionLabel->setStyleSheet("color: #546E7A; font-size: 32px; margin-bottom: 5px;");
    layout->addWidget(instructionLabel);

    // Professional color scheme
    auto createButton = [](const QString& text, const QString& color, const QString& hoverColor) -> QPushButton* {
        QPushButton* btn = new QPushButton(text);
        btn->setMinimumHeight(65);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(
            "QPushButton {"
            "   background-color: %1;"
            "   color: white;"
            "   font-weight: bold;"
            "   font-size: 32px;"
            "   border: none;"
            "   border-radius: 8px;"
            "   padding: 12px;"
            "   text-align: left;"
            "   padding-left: 20px;"
            "}"
            "QPushButton:hover {"
            "   background-color: %2;"
            "}"
            "QPushButton:pressed {"
            "   background-color: %2;"
            "   padding-top: 14px;"
            "   padding-bottom: 10px;"
            "}"
        ).arg(color, hoverColor));
        return btn;
    };

    tracheostomyButton_ = createButton("Tracheostomy", "#e8e8e8", "#d0d0d0");
    tracheostomyButton_->setStyleSheet(tracheostomyButton_->styleSheet().replace("color: white;", "color: #333333;"));
    newTracheostomyButton_ = createButton("New Tracheostomy", "#ff66cc", "#dd44aa");
    difficultAirwayButton_ = createButton("Difficult Airway", "#ffc000", "#dd9900");
    difficultAirwayButton_->setStyleSheet(difficultAirwayButton_->styleSheet().replace("color: white;", "color: #333333;"));
    ltrButton_ = createButton("Laryngotracheal Reconstruction (LTR)", "#00b0f0", "#0090cc");

    layout->addWidget(tracheostomyButton_);
    layout->addWidget(newTracheostomyButton_);
    layout->addWidget(difficultAirwayButton_);
    layout->addWidget(ltrButton_);
    layout->addStretch();

    connect(tracheostomyButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
    connect(newTracheostomyButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
    connect(difficultAirwayButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
    connect(ltrButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
}

void CaseSelectionView::setupExistingCaseSection()
{
    existingCaseGroup_ = new QGroupBox("Load Existing Case");
    existingCaseGroup_->setStyleSheet(
        "QGroupBox {"
        "   background-color: white;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 12px;"
        "   padding: 20px;"
        "   margin-top: 15px;"
        "   font-size: 32px;"
        "   font-weight: bold;"
        "   color: #2C3E50;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   subcontrol-position: top left;"
        "   padding: 5px 10px;"
        "   background-color: white;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(existingCaseGroup_);
    layout->setSpacing(12);
    layout->setContentsMargins(15, 25, 15, 15);

    QLabel* instructionLabel = new QLabel("Recent cases:");
    instructionLabel->setFont(StyleManager::instance().getBodyFont());
    instructionLabel->setStyleSheet("color: #546E7A; font-size: 32px; margin-bottom: 5px;");
    layout->addWidget(instructionLabel);

    recentCasesList_ = new QListWidget();
    recentCasesList_->setMinimumHeight(250);
    recentCasesList_->setStyleSheet(
        "QListWidget {"
        "   background-color: #F5F5F5;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   font-size: 32px;"
        "}"
        "QListWidget::item {"
        "   background-color: white;"
        "   border: 1px solid #E8E8E8;"
        "   border-radius: 6px;"
        "   padding: 14px;"
        "   margin: 4px 2px;"
        "   color: #37474F;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #E3F2FD;"
        "   border-color: #90CAF9;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #BBDEFB;"
        "   border-color: #64B5F6;"
        "   color: #1565C0;"
        "}"
    );
    layout->addWidget(recentCasesList_);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    auto createActionButton = [](const QString& text, const QString& color, const QString& hoverColor) -> QPushButton* {
        QPushButton* btn = new QPushButton(text);
        btn->setMinimumHeight(50);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(
            "QPushButton {"
            "   background-color: %1;"
            "   color: white;"
            "   font-weight: bold;"
            "   font-size: 32px;"
            "   border: none;"
            "   border-radius: 8px;"
            "   padding: 10px 20px;"
            "}"
            "QPushButton:hover {"
            "   background-color: %2;"
            "}"
            "QPushButton:pressed {"
            "   background-color: %2;"
            "   padding-top: 12px;"
            "   padding-bottom: 8px;"
            "}"
        ).arg(color, hoverColor));
        return btn;
    };

    loadCaseButton_ = createActionButton("Load from File...", "#607D8B", "#546E7A");
    refreshButton_ = createActionButton("Refresh", "#78909C", "#607D8B");

    buttonLayout->addWidget(loadCaseButton_);
    buttonLayout->addWidget(refreshButton_);

    layout->addLayout(buttonLayout);

    connect(recentCasesList_, &QListWidget::itemDoubleClicked, this, &CaseSelectionView::onRecentCaseDoubleClicked);
    connect(loadCaseButton_, &QPushButton::clicked, this, &CaseSelectionView::onLoadCaseClicked);
    connect(refreshButton_, &QPushButton::clicked, this, &CaseSelectionView::refreshRecentCases);
}

void CaseSelectionView::updateStyles()
{
    QFont bodyFont = StyleManager::instance().getBodyFont();
    QFont headerFont = StyleManager::instance().getHeaderFont();
    
    titleLabel_->setFont(headerFont);
    
    tracheostomyButton_->setFont(bodyFont);
    newTracheostomyButton_->setFont(bodyFont);
    difficultAirwayButton_->setFont(bodyFont);
    ltrButton_->setFont(bodyFont);
    
    loadCaseButton_->setFont(bodyFont);
    refreshButton_->setFont(bodyFont);
    recentCasesList_->setFont(bodyFont);
}

void CaseSelectionView::onNewCaseClicked()
{
    emit newCaseRequested(selectedCaseType_);
}

void CaseSelectionView::onLoadCaseClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        "Load Case", 
        Application::instance().getCaseManager()->getBasePath(),
        "JSON files (*.json)");
    
    if (!fileName.isEmpty()) {
        emit existingCaseSelected(fileName);
    }
}

void CaseSelectionView::onCaseTypeButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    if (button == tracheostomyButton_) {
        selectedCaseType_ = CaseType::Tracheostomy;
    } else if (button == newTracheostomyButton_) {
        selectedCaseType_ = CaseType::NewTracheostomy;
    } else if (button == difficultAirwayButton_) {
        selectedCaseType_ = CaseType::DifficultAirway;
    } else if (button == ltrButton_) {
        selectedCaseType_ = CaseType::LTR;
    }
    
    emit newCaseRequested(selectedCaseType_);
}

void CaseSelectionView::onRecentCaseDoubleClicked()
{
    QListWidgetItem* item = recentCasesList_->currentItem();
    if (item) {
        QString filePath = item->data(Qt::UserRole).toString();
        emit existingCaseSelected(filePath);
    }
}

void CaseSelectionView::refreshRecentCases()
{
    loadRecentCases();
}

void CaseSelectionView::loadRecentCases()
{
    recentCasesList_->clear();
    
    QStringList recentCases = Application::instance().getCaseManager()->getRecentCases();
    
    for (const QString& filePath : recentCases) {
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists()) {
            QString displayName = formatCaseDisplayName(filePath);
            QListWidgetItem* item = new QListWidgetItem(displayName);
            item->setData(Qt::UserRole, filePath);
            recentCasesList_->addItem(item);
        }
    }
}

QString CaseSelectionView::formatCaseDisplayName(const QString& filePath) const
{
    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.baseName();
    
    QDateTime lastModified = fileInfo.lastModified();
    QString timeString = lastModified.toString("yyyy-MM-dd hh:mm");
    
    QString caseType = "Unknown";
    if (filePath.contains("tracheostomy")) {
        caseType = "Tracheostomy";
    } else if (filePath.contains("new_tracheostomy")) {
        caseType = "New Tracheostomy";
    } else if (filePath.contains("difficult_airway")) {
        caseType = "Difficult Airway";
    } else if (filePath.contains("ltr")) {
        caseType = "LTR";
    }
    
    // Try to load patient info from the case file
    QString patientName = "";
    Case caseData;
    if (Application::instance().getCaseManager()->loadCase(filePath, caseData)) {
        PatientInfo patient = caseData.getPatient();
        if (!patient.firstName.isEmpty() || !patient.lastName.isEmpty()) {
            patientName = QString("%1 %2").arg(patient.firstName).arg(patient.lastName).trimmed();
            if (!patientName.isEmpty()) {
                patientName += " - ";
            }
        }
    }
    
    return QString("%1 - %2%3 (%4)").arg(caseType).arg(patientName).arg(baseName).arg(timeString);
}

void CaseSelectionView::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    // Auto-refresh the case list when the view is shown
    refreshRecentCases();
}