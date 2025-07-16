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
    mainLayout_ = new QVBoxLayout(this);
    
    titleLabel_ = new QLabel("Safe Airway - Case Selection");
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setFont(StyleManager::instance().getHeaderFont());
    mainLayout_->addWidget(titleLabel_);
    
    logoLabel_ = new QLabel();
    logoLabel_->setAlignment(Qt::AlignCenter);
    logoLabel_->setMaximumHeight(100);
    logoLabel_->setScaledContents(true);
    
    // Load and set the Nemours logo
    QPixmap logo(":/images/nemours-logo.png");
    if (!logo.isNull()) {
        logoLabel_->setPixmap(logo.scaled(200, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel_->setText("Nemours Children's Health");
        logoLabel_->setStyleSheet("font-weight: bold; color: #0066CC; font-size: 16px;");
    }
    mainLayout_->addWidget(logoLabel_);
    
    QHBoxLayout* contentLayout = new QHBoxLayout();
    
    setupNewCaseSection();
    setupExistingCaseSection();
    
    contentLayout->addWidget(newCaseGroup_);
    contentLayout->addWidget(existingCaseGroup_);
    
    mainLayout_->addLayout(contentLayout);
    mainLayout_->addStretch();
    
    setLayout(mainLayout_);
}

void CaseSelectionView::setupNewCaseSection()
{
    newCaseGroup_ = new QGroupBox("Create New Case");
    QVBoxLayout* layout = new QVBoxLayout(newCaseGroup_);
    
    QLabel* instructionLabel = new QLabel("Select the type of case you want to create:");
    instructionLabel->setFont(StyleManager::instance().getBodyFont());
    layout->addWidget(instructionLabel);
    
    tracheostomyButton_ = new QPushButton("Tracheostomy");
    tracheostomyButton_->setMinimumHeight(50);
    tracheostomyButton_->setStyleSheet(QString("QPushButton { background-color: %1; color: white; font-weight: bold; }")
                                      .arg(StyleManager::instance().getFormColour(CaseType::Tracheostomy).name()));
    
    newTracheostomyButton_ = new QPushButton("New Tracheostomy");
    newTracheostomyButton_->setMinimumHeight(50);
    newTracheostomyButton_->setStyleSheet(QString("QPushButton { background-color: %1; color: white; font-weight: bold; }")
                                         .arg(StyleManager::instance().getFormColour(CaseType::NewTracheostomy).name()));
    
    difficultAirwayButton_ = new QPushButton("Difficult Airway");
    difficultAirwayButton_->setMinimumHeight(50);
    difficultAirwayButton_->setStyleSheet(QString("QPushButton { background-color: %1; color: black; font-weight: bold; }")
                                         .arg(StyleManager::instance().getFormColour(CaseType::DifficultAirway).name()));
    
    ltrButton_ = new QPushButton("Laryngotracheal Reconstruction (LTR)");
    ltrButton_->setMinimumHeight(50);
    ltrButton_->setStyleSheet(QString("QPushButton { background-color: %1; color: white; font-weight: bold; }")
                             .arg(StyleManager::instance().getFormColour(CaseType::LTR).name()));
    
    layout->addWidget(tracheostomyButton_);
    layout->addWidget(newTracheostomyButton_);
    layout->addWidget(difficultAirwayButton_);
    layout->addWidget(ltrButton_);
    
    connect(tracheostomyButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
    connect(newTracheostomyButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
    connect(difficultAirwayButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
    connect(ltrButton_, &QPushButton::clicked, this, &CaseSelectionView::onCaseTypeButtonClicked);
}

void CaseSelectionView::setupExistingCaseSection()
{
    existingCaseGroup_ = new QGroupBox("Load Existing Case");
    QVBoxLayout* layout = new QVBoxLayout(existingCaseGroup_);
    
    QLabel* instructionLabel = new QLabel("Recent cases:");
    instructionLabel->setFont(StyleManager::instance().getBodyFont());
    layout->addWidget(instructionLabel);
    
    recentCasesList_ = new QListWidget();
    recentCasesList_->setMinimumHeight(200);
    layout->addWidget(recentCasesList_);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    loadCaseButton_ = new QPushButton("Load from File...");
    refreshButton_ = new QPushButton("Refresh");
    
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