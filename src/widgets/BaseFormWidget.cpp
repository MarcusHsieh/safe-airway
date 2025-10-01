#include "BaseFormWidget.h"
#include "utils/StyleManager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPageLayout>
#include <QTableWidgetItem>
#include <QSplitter>
#include <QDebug>
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>

BaseFormWidget::BaseFormWidget(CaseType caseType, QWidget* parent)
    : QWidget(parent)
    , caseType_(caseType)
    , frozen_(false)
    , justSaved_(true)
    , screenSize_(QGuiApplication::primaryScreen()->availableSize())
    , scrollArea_(nullptr)
    , contentWidget_(nullptr)
    , mainLayout_(nullptr)
    , contentLayout_(nullptr)
    , rightLayout_(nullptr)
    , formFieldsLayout_(nullptr)
    , headerLabel_(nullptr)
    , logoLabel_(nullptr)
    , patientInfoWidget_(nullptr)
    , tubeSpecWidget_(nullptr)
    , addRowButton_(nullptr)
    , removeRowButton_(nullptr)
    , moveUpButton_(nullptr)
    , moveDownButton_(nullptr)
    , emergencyPanelOverlay_(nullptr)
    , emergencyButton_(nullptr)
    , decisionBoxGroup_(nullptr)
    , maskVentilateCheckBox_(nullptr)
    , intubateAboveCheckBox_(nullptr)
    , intubateStomaCheckBox_(nullptr)
    , sidePanelGroup_(nullptr)
    , suctionSizeSpinBox_(nullptr)
    , suctionDepthEdit_(nullptr)
    , specialCommentsEdit_(nullptr)
    , freezeButton_(nullptr)
    , saveButton_(nullptr)
    , printButton_(nullptr)
    , backButton_(nullptr)
{
    currentCase_ = Case();  // Use default constructor to generate UUID
    currentCase_.setCaseType(caseType);
    setupUI();
    connectSignals();
    updateStyles();
}

void BaseFormWidget::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    
    scrollArea_ = new QScrollArea();
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    contentWidget_ = new QWidget();
    contentLayout_ = new QVBoxLayout(contentWidget_);
    // Use percentage-based spacing and margins
    int spacing = screenSize_.height() * 0.015; // 1.5% of screen height
    int margin = screenSize_.width() * 0.015; // 1.5% of screen width
    
    contentLayout_->setSpacing(spacing);
    contentLayout_->setContentsMargins(margin, margin, margin, margin);
    
    setupHeader();
    // Header now contains both logo and title in horizontal layout
    QWidget* headerWidget = new QWidget();
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setSpacing(20);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    
    // Add logo to the left of the title
    headerLayout->addWidget(logoLabel_);
    headerLayout->addWidget(headerLabel_, 1); // Give title more space
    
    contentLayout_->addWidget(headerWidget);
    
    // Create two main columns layout
    QHBoxLayout* mainContentLayout = new QHBoxLayout();
    
    // COLUMN 1: Patient Info, Decision Boxes, and Suction & Comments
    QVBoxLayout* leftColumnLayout = new QVBoxLayout();
    
    // Patient Info (moved from top to left column)
    patientInfoWidget_ = new PatientInfoWidget();
    leftColumnLayout->addWidget(patientInfoWidget_);
    
    // Decision boxes (moved from Column 2)
    setupDecisionBoxes();
    leftColumnLayout->addWidget(decisionBoxGroup_);
    
    // Suction & Comments panel (moved from Column 2 for better balance)
    setupSidePanel();
    leftColumnLayout->addWidget(sidePanelGroup_);
    
    // Create emergency panel overlay (initially hidden)
    emergencyPanelOverlay_ = new EmergencyPanelOverlay(this);
    
    leftColumnLayout->addStretch();
    
    // COLUMN 2: Tube Specification Widget and Tracheostomy Information
    QVBoxLayout* rightColumnLayout = new QVBoxLayout();
    
    // Tube specification widget (full column width)
    tubeSpecWidget_ = new TubeSpecificationWidget();
    rightColumnLayout->addWidget(tubeSpecWidget_);
    
    // Form-specific fields (moved from Column 1 for logical grouping with tube specs)
    QWidget* formFieldsWidget = new QWidget();
    formFieldsLayout_ = new QVBoxLayout(formFieldsWidget);
    formFieldsLayout_->setContentsMargins(0, 10, 0, 0);
    rightColumnLayout->addWidget(formFieldsWidget);
    
    rightColumnLayout->addStretch();
    
    // Add the two columns with better balanced proportions (45:55)
    mainContentLayout->addLayout(leftColumnLayout, 45);  // Column 1 - 45% of screen
    mainContentLayout->addLayout(rightColumnLayout, 55); // Column 2 - 55% of screen
    
    contentLayout_->addLayout(mainContentLayout);
    
    // Add bottom action buttons bar
    setupActionButtons();
    QHBoxLayout* bottomButtonLayout = new QHBoxLayout();
    bottomButtonLayout->setSpacing(15);
    bottomButtonLayout->setContentsMargins(20, 10, 20, 10);
    
    // Emergency scenarios button (moved from Column 1)
    emergencyButton_ = new QPushButton("Emergency Scenarios");
    emergencyButton_->setMinimumHeight(60);
    emergencyButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #DC143C;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 18px;"
        "   border: 2px solid #DC143C;"
        "   border-radius: 8px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #B22222;"
        "   border-color: #B22222;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #8B0000;"
        "}"
    );
    
    bottomButtonLayout->addWidget(freezeButton_);
    bottomButtonLayout->addWidget(emergencyButton_);
    bottomButtonLayout->addStretch();
    bottomButtonLayout->addWidget(displayModeButton_);
    bottomButtonLayout->addWidget(saveButton_);
    bottomButtonLayout->addWidget(printButton_);
    bottomButtonLayout->addWidget(backButton_);
    
    contentLayout_->addLayout(bottomButtonLayout);
    
    // Connect emergency button signal now that it's created
    connect(emergencyButton_, &QPushButton::clicked, this, [this]() {
        emergencyPanelOverlay_->showOverlay();
    });
    
    scrollArea_->setWidget(contentWidget_);
    mainLayout_->addWidget(scrollArea_);
    
    setLayout(mainLayout_);
}

void BaseFormWidget::finishSetup()
{
    // Now setup form-specific fields - they will be added to formFieldsLayout_
    setupFormSpecificFields();
    
    // Add stretch to form fields layout to push content to top
    if (formFieldsLayout_) {
        formFieldsLayout_->addStretch();
    }
}

void BaseFormWidget::setupHeader()
{
    // Create logo label - smaller since it's now beside the title
    logoLabel_ = new QLabel();
    logoLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    logoLabel_->setMaximumHeight(60);
    logoLabel_->setMaximumWidth(150);
    logoLabel_->setScaledContents(true);
    
    // Load and set the Nemours logo
    QPixmap logo(":/images/nemours-logo.png");
    if (!logo.isNull()) {
        logoLabel_->setPixmap(logo.scaled(150, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel_->setText("Nemours");
        logoLabel_->setStyleSheet("font-weight: bold; color: #0066CC; font-size: 16px;");
    }
    
    headerLabel_ = new QLabel();
    headerLabel_->setObjectName("headerLabel");
    headerLabel_->setAlignment(Qt::AlignCenter);
    headerLabel_->setMinimumHeight(60);
    
    QString formTitle;
    switch (caseType_) {
    case CaseType::Tracheostomy:
        formTitle = "TRACHEOSTOMY FORM";
        break;
    case CaseType::NewTracheostomy:
        formTitle = "NEW TRACHEOSTOMY FORM";
        break;
    case CaseType::DifficultAirway:
        formTitle = "DIFFICULT AIRWAY FORM";
        break;
    case CaseType::LTR:
        formTitle = "LARYNGOTRACHEAL RECONSTRUCTION (LTR) FORM";
        break;
    }
    
    headerLabel_->setText(formTitle);
}


void BaseFormWidget::setupSpecTableButtons()
{
    // Table manipulation buttons removed for single-row mode
    // All buttons set to nullptr to avoid crashes
    addRowButton_ = nullptr;
    removeRowButton_ = nullptr;
    moveUpButton_ = nullptr;
    moveDownButton_ = nullptr;
}

void BaseFormWidget::setupDecisionBoxes()
{
    decisionBoxGroup_ = new QGroupBox("Emergency Airway Options");
    decisionBoxGroup_->setFont(StyleManager::instance().getGroupBoxFont());
    // Use percentage-based width for decision box
    int decisionBoxWidth = screenSize_.width() * 0.25; // 25% of screen width
    decisionBoxGroup_->setMinimumWidth(decisionBoxWidth);
    QVBoxLayout* layout = new QVBoxLayout(decisionBoxGroup_);
    layout->setSpacing(15); // More spacing between checkboxes
    
    maskVentilateCheckBox_ = new QCheckBox("Mask Ventilate");
    intubateAboveCheckBox_ = new QCheckBox("Intubate Above");
    intubateStomaCheckBox_ = new QCheckBox("Intubate Stoma");
    
    // Apply larger font to decision boxes
    QFont decisionFont = StyleManager::instance().getDecisionBoxFont();
    maskVentilateCheckBox_->setFont(decisionFont);
    intubateAboveCheckBox_->setFont(decisionFont);
    intubateStomaCheckBox_->setFont(decisionFont);
    
    // Make checkboxes larger using percentage-based height
    int checkboxHeight = screenSize_.height() * 0.04; // 4% of screen height
    maskVentilateCheckBox_->setMinimumHeight(checkboxHeight);
    intubateAboveCheckBox_->setMinimumHeight(checkboxHeight);
    intubateStomaCheckBox_->setMinimumHeight(checkboxHeight);
    
    layout->addWidget(maskVentilateCheckBox_);
    layout->addWidget(intubateAboveCheckBox_);
    layout->addWidget(intubateStomaCheckBox_);
}

void BaseFormWidget::setupSidePanel()
{
    sidePanelGroup_ = new QGroupBox("Suction & Comments");
    sidePanelGroup_->setFont(StyleManager::instance().getGroupBoxFont());
    QVBoxLayout* layout = new QVBoxLayout(sidePanelGroup_);
    
    // Suction Size - vertical layout
    QLabel* suctionSizeLabel = new QLabel("Suction Size:");
    suctionSizeLabel->setFont(StyleManager::instance().getKeyElementFont());
    layout->addWidget(suctionSizeLabel);
    
    suctionSizeSpinBox_ = new QSpinBox();
    // Use percentage-based sizing for suction spinbox
    int suctionWidth = screenSize_.width() * 0.15; // 15% of screen width
    int suctionHeight = screenSize_.height() * 0.04; // 4% of screen height
    suctionSizeSpinBox_->setMinimumWidth(suctionWidth);
    suctionSizeSpinBox_->setMinimumHeight(suctionHeight);
    suctionSizeSpinBox_->setRange(1, 20);
    suctionSizeSpinBox_->setValue(6);
    suctionSizeSpinBox_->setAlignment(Qt::AlignLeft);
    suctionSizeSpinBox_->setFont(StyleManager::instance().getKeyElementFont());
    layout->addWidget(suctionSizeSpinBox_);
    
    // Suction Depth - vertical layout
    QLabel* suctionDepthLabel = new QLabel("Depth:");
    suctionDepthLabel->setFont(StyleManager::instance().getKeyElementFont());
    layout->addWidget(suctionDepthLabel);
    
    suctionDepthEdit_ = new QLineEdit();
    suctionDepthEdit_->setAlignment(Qt::AlignLeft);
    suctionDepthEdit_->setPlaceholderText("e.g., 5 cm");
    suctionDepthEdit_->setMinimumHeight(suctionHeight);
    layout->addWidget(suctionDepthEdit_);
    
    layout->addWidget(new QLabel("Special Comments:"));
    specialCommentsEdit_ = new QTextEdit();
    // Use percentage-based height for comments area
    int commentsMinHeight = screenSize_.height() * 0.12; // 12% of screen height
    int commentsMaxHeight = screenSize_.height() * 0.16; // 16% of screen height
    specialCommentsEdit_->setMinimumHeight(commentsMinHeight);
    specialCommentsEdit_->setMaximumHeight(commentsMaxHeight);
    
    // Disable horizontal scrollbar to force wrapping
    specialCommentsEdit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    specialCommentsEdit_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Custom text wrapping at 67 characters
    connect(specialCommentsEdit_, &QTextEdit::textChanged, [this]() {
        if (!specialCommentsEdit_) return;
        
        // Block signals to prevent recursion
        specialCommentsEdit_->blockSignals(true);
        
        QTextCursor cursor = specialCommentsEdit_->textCursor();
        QString text = specialCommentsEdit_->toPlainText();
        
        // Split text into lines and process each line
        QStringList lines = text.split('\n');
        QStringList processedLines;
        
        for (const QString& line : lines) {
            if (line.length() <= 67) {
                processedLines << line;
            } else {
                // Split long lines at 67 characters, preferring word boundaries
                QString remainingText = line;
                while (remainingText.length() > 67) {
                    int breakPoint = 67;
                    // Try to break at a space near the 67-character limit
                    for (int i = 67; i >= 50; --i) {
                        if (i < remainingText.length() && remainingText[i] == ' ') {
                            breakPoint = i;
                            break;
                        }
                    }
                    
                    processedLines << remainingText.left(breakPoint);
                    remainingText = remainingText.mid(breakPoint).trimmed();
                }
                if (!remainingText.isEmpty()) {
                    processedLines << remainingText;
                }
            }
        }
        
        QString newText = processedLines.join('\n');
        if (newText != text) {
            specialCommentsEdit_->setPlainText(newText);
            
            // Set cursor position to the end of the text (after the last character typed)
            cursor.setPosition(newText.length());
            specialCommentsEdit_->setTextCursor(cursor);
        }
        
        specialCommentsEdit_->blockSignals(false);
    });
    
    layout->addWidget(specialCommentsEdit_);
    
    // Add some spacing to ensure case-specific fields are visible
    layout->addSpacing(10);
}

void BaseFormWidget::setupActionButtons()
{
    freezeButton_ = new QPushButton("Freeze Mode");
    freezeButton_->setCheckable(true);
    freezeButton_->setStyleSheet("QPushButton:checked { background-color: #DC143C; color: white; }");
    
    saveButton_ = new QPushButton("Save Case");
    printButton_ = new QPushButton("Print");
    backButton_ = new QPushButton("Back");
    displayModeButton_ = new QPushButton("Display Mode");
    displayModeButton_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");

    connect(freezeButton_, &QPushButton::toggled, this, &BaseFormWidget::onFreezeClicked);
    connect(saveButton_, &QPushButton::clicked, this, &BaseFormWidget::onSaveClicked);
    connect(printButton_, &QPushButton::clicked, this, &BaseFormWidget::onPrintClicked);
    connect(backButton_, &QPushButton::clicked, this, &BaseFormWidget::onBackClicked);
    connect(displayModeButton_, &QPushButton::clicked, this, &BaseFormWidget::onDisplayModeClicked);
}

void BaseFormWidget::connectSignals()
{
    connect(patientInfoWidget_, &PatientInfoWidget::patientInfoChanged, this, &BaseFormWidget::onPatientInfoChanged);
    // Emergency button connection will be added after setupUI is complete
    connect(emergencyPanelOverlay_, &EmergencyPanelOverlay::scenarioSelected, this, &BaseFormWidget::onEmergencyScenarioSelected);
    connect(suctionSizeSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, &BaseFormWidget::onSuctionSizeChanged);
    connect(tubeSpecWidget_, &TubeSpecificationWidget::specificationChanged, this, &BaseFormWidget::onFormFieldChanged);
    
    connect(maskVentilateCheckBox_, &QCheckBox::toggled, this, &BaseFormWidget::onFormFieldChanged);
    connect(intubateAboveCheckBox_, &QCheckBox::toggled, this, &BaseFormWidget::onFormFieldChanged);
    connect(intubateStomaCheckBox_, &QCheckBox::toggled, this, &BaseFormWidget::onFormFieldChanged);
    connect(suctionDepthEdit_, &QLineEdit::textChanged, this, &BaseFormWidget::onFormFieldChanged);
    connect(specialCommentsEdit_, &QTextEdit::textChanged, this, &BaseFormWidget::onFormFieldChanged);
}

void BaseFormWidget::updateStyles()
{
    StyleManager::instance().applyFormStyle(this, caseType_);
}

void BaseFormWidget::setCase(const Case& case_)
{
    currentCase_ = case_;
    loadFormData();
    justSaved_ = true; // Loading a case means it's saved
}

Case BaseFormWidget::getCase() const
{
    Case case_ = currentCase_;
    
    // Update case with current tube specification
    SpecificationTableRow spec = tubeSpecWidget_->getSpecification();
    QList<SpecificationTableRow> specs;
    if (!spec.size.isEmpty()) {
        SpecificationTableRow row;
        row.makeModel = spec.makeModel;
        row.size = spec.size;
        row.type = spec.type;
        row.cuff = spec.cuff;
        row.innerDiameter = spec.innerDiameter;
        row.outerDiameter = spec.outerDiameter;
        row.length = spec.length;
        row.reorderNumber = spec.reorderNumber;
        specs.append(row);
    }
    case_.setSpecTable(specs);
    
    // Now update all form fields
    case_.setPatient(patientInfoWidget_->getPatientInfo());
    case_.setEmergencyScenario(emergencyPanelOverlay_->getSelectedScenario());
    
    SuctionInfo suction;
    suction.size = suctionSizeSpinBox_->value();
    suction.depth = suctionDepthEdit_->text();
    case_.setSuction(suction);
    
    DecisionBox decision;
    decision.maskVentilate = maskVentilateCheckBox_->isChecked();
    decision.intubateAbove = intubateAboveCheckBox_->isChecked();
    decision.intubateStoma = intubateStomaCheckBox_->isChecked();
    case_.setDecisionBox(decision);
    
    case_.setSpecialComments(specialCommentsEdit_->toPlainText());
    case_.setUpdatedAt(QDateTime::currentDateTime());
    
    return case_;
}

void BaseFormWidget::setFrozen(bool frozen)
{
    frozen_ = frozen;
    
    patientInfoWidget_->setFrozen(frozen);
    emergencyButton_->setEnabled(!frozen);
    
    tubeSpecWidget_->setReadOnly(frozen);
    
    decisionBoxGroup_->setEnabled(!frozen);
    suctionSizeSpinBox_->setEnabled(!frozen);
    suctionDepthEdit_->setReadOnly(frozen);
    specialCommentsEdit_->setReadOnly(frozen);
    
    saveButton_->setEnabled(!frozen);
    
    if (frozen) {
        StyleManager::instance().applyFreezeOverlay(contentWidget_, true);
        freezeButton_->setChecked(true);
        freezeButton_->setText("Unfreeze");
    } else {
        StyleManager::instance().applyFreezeOverlay(contentWidget_, false);
        freezeButton_->setChecked(false);
        freezeButton_->setText("Freeze Mode");
        // Don't call updateStyles() as it resets font size - styles are preserved automatically
    }
    
    emit freezeModeToggled(frozen);
}

bool BaseFormWidget::isFormValid() const
{
    return getValidationErrors().isEmpty();
}

QStringList BaseFormWidget::getValidationErrors() const
{
    QStringList errors;
    
    PatientInfo patient = patientInfoWidget_->getPatientInfo();
    if (patient.firstName.isEmpty()) {
        errors << "Patient first name is required";
    }
    
    if (!tubeSpecWidget_->isValid()) {
        errors.append(tubeSpecWidget_->getValidationErrors());
    }
    
    return errors;
}

void BaseFormWidget::clear()
{
    patientInfoWidget_->clear();
    emergencyPanelOverlay_->setSelectedScenario("");
    
    tubeSpecWidget_->clear();
    
    maskVentilateCheckBox_->setChecked(false);
    intubateAboveCheckBox_->setChecked(false);
    intubateStomaCheckBox_->setChecked(false);
    
    suctionSizeSpinBox_->setValue(6);
    suctionDepthEdit_->clear();
    specialCommentsEdit_->clear();
    
    currentCase_ = Case();  // Use default constructor to generate UUID
    currentCase_.setCaseType(caseType_);
    justSaved_ = true; // New empty case is considered "saved"
}





void BaseFormWidget::loadFormData()
{
    patientInfoWidget_->setPatientInfo(currentCase_.getPatient());
    emergencyPanelOverlay_->setSelectedScenario(currentCase_.getEmergencyScenario());
    
    updateTableFromCase();
    
    SuctionInfo suction = currentCase_.getSuction();
    suctionSizeSpinBox_->setValue(suction.size);
    suctionDepthEdit_->setText(suction.depth);
    
    DecisionBox decision = currentCase_.getDecisionBox();
    maskVentilateCheckBox_->setChecked(decision.maskVentilate);
    intubateAboveCheckBox_->setChecked(decision.intubateAbove);
    intubateStomaCheckBox_->setChecked(decision.intubateStoma);
    
    specialCommentsEdit_->setPlainText(currentCase_.getSpecialComments());
    
    // Data loaded successfully
}

void BaseFormWidget::updateTableFromCase()
{
    QList<SpecificationTableRow> specs = currentCase_.getSpecTable();
    
    // Load first specification into the widget (if any)
    if (!specs.isEmpty()) {
        const SpecificationTableRow& spec = specs.first();
        
        SpecificationTableRow tubeSpec;
        tubeSpec.makeModel = spec.makeModel;
        tubeSpec.size = spec.size;
        tubeSpec.type = spec.type;
        tubeSpec.cuff = spec.cuff;
        tubeSpec.innerDiameter = spec.innerDiameter;
        tubeSpec.outerDiameter = spec.outerDiameter;
        tubeSpec.length = spec.length;
        tubeSpec.reorderNumber = spec.reorderNumber;
        
        tubeSpecWidget_->setSpecification(tubeSpec);
    } else {
        tubeSpecWidget_->clear();
    }
}


void BaseFormWidget::onSaveClicked()
{
    if (!isFormValid()) {
        QStringList errors = getValidationErrors();
        QMessageBox::warning(this, "Validation Error", 
                           "Please fix the following errors:\n\n" + errors.join("\n"));
        return;
    }
    
    // Case data is automatically updated through getCase() method
    saveFormData();
    justSaved_ = true;
    emit saveRequested();
}

void BaseFormWidget::onFreezeClicked()
{
    setFrozen(!frozen_);
}

void BaseFormWidget::onPrintClicked()
{
    printForm();
}

void BaseFormWidget::onBackClicked()
{
    // Navigate back to case selection by emitting a signal
    // The MainWindow will handle showing the case selection view
    emit backRequested();
}

void BaseFormWidget::onDisplayModeClicked()
{
    // Switch to display mode by emitting a signal
    // The MainWindow will handle showing the appropriate display view
    emit displayModeRequested();
}

void BaseFormWidget::onPatientInfoChanged()
{
    justSaved_ = false;
    emit formChanged();
}

void BaseFormWidget::onEmergencyScenarioSelected(const QString& scenario)
{
    updateEmergencyAdvice();
    emit emergencyScenarioChanged(scenario);
    emit formChanged();
}

void BaseFormWidget::onSuctionSizeChanged(int size)
{
    emergencyPanelOverlay_->setSuctionSize(size);
    emit formChanged();
}


void BaseFormWidget::onFormFieldChanged()
{
    justSaved_ = false;
    currentCase_.setUpdatedAt(QDateTime::currentDateTime());
    emit formChanged();
}

void BaseFormWidget::saveFormData()
{
    currentCase_ = getCase();
}

void BaseFormWidget::updateEmergencyAdvice()
{
    
}


void BaseFormWidget::printForm()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);
    
    QPrintDialog printDialog(&printer, this);
    printDialog.setWindowTitle("Print Form");
    
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        
        // Get the size of the widget and the page
        QSize widgetSize = this->size();
        QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);
        
        // Calculate scale factor to fit the entire form on the page
        double scaleX = pageRect.width() / widgetSize.width();
        double scaleY = pageRect.height() / widgetSize.height();
        double scale = qMin(scaleX, scaleY);
        
        // Apply scaling
        painter.scale(scale, scale);
        
        // Render the widget
        render(&painter);
    }
}