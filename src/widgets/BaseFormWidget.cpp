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

BaseFormWidget::BaseFormWidget(CaseType caseType, QWidget* parent)
    : QWidget(parent)
    , caseType_(caseType)
    , frozen_(false)
    , justSaved_(true)
    , scrollArea_(nullptr)
    , contentWidget_(nullptr)
    , mainLayout_(nullptr)
    , contentLayout_(nullptr)
    , rightLayout_(nullptr)
    , formFieldsLayout_(nullptr)
    , headerLabel_(nullptr)
    , logoLabel_(nullptr)
    , patientInfoWidget_(nullptr)
    , specificationTable_(nullptr)
    , makeModelEdit_(nullptr)
    , addRowButton_(nullptr)
    , removeRowButton_(nullptr)
    , moveUpButton_(nullptr)
    , moveDownButton_(nullptr)
    , emergencyPanel_(nullptr)
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
    currentCase_ = Case(QString(), caseType);
    setupUI();
    connectSignals();
    updateStyles();
}

void BaseFormWidget::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    
    scrollArea_ = new QScrollArea();
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    contentWidget_ = new QWidget();
    contentLayout_ = new QVBoxLayout(contentWidget_);
    contentLayout_->setSpacing(15);
    contentLayout_->setContentsMargins(20, 20, 20, 20);
    
    setupHeader();
    contentLayout_->addWidget(logoLabel_);
    contentLayout_->addWidget(headerLabel_);
    
    // Create two main columns layout
    QHBoxLayout* mainContentLayout = new QHBoxLayout();
    
    // COLUMN 1: Patient Info, Make/Model and Emergency Scenarios
    QVBoxLayout* leftColumnLayout = new QVBoxLayout();
    
    // Patient Info (moved from top to left column)
    patientInfoWidget_ = new PatientInfoWidget();
    leftColumnLayout->addWidget(patientInfoWidget_);
    
    // Make/Model text area
    QLabel* makeModelLabel = new QLabel("Make/Model:");
    makeModelLabel->setFont(StyleManager::instance().getBodyFont());
    leftColumnLayout->addWidget(makeModelLabel);
    
    makeModelEdit_ = new QTextEdit();
    makeModelEdit_->setMinimumHeight(100);
    makeModelEdit_->setMaximumHeight(150);
    makeModelEdit_->setPlaceholderText("Enter device make and model information...");
    makeModelEdit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    makeModelEdit_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    leftColumnLayout->addWidget(makeModelEdit_);
    
    // Emergency scenarios
    emergencyPanel_ = new EmergencyPanel();
    leftColumnLayout->addWidget(emergencyPanel_);
    leftColumnLayout->addStretch();
    
    // COLUMN 2: Table and other components
    QVBoxLayout* rightColumnLayout = new QVBoxLayout();
    
    // Specification table (without Make/Model column)
    setupSpecificationTable();
    rightColumnLayout->addWidget(specificationTable_);
    
    setupSpecTableButtons(); // Initialize button pointers to nullptr
    
    // Suction & Comments
    setupSidePanel();
    rightColumnLayout->addWidget(sidePanelGroup_);
    
    // Decision boxes and form-specific fields in horizontal layout
    QHBoxLayout* decisionAndFormLayout = new QHBoxLayout();
    
    // Decision boxes
    setupDecisionBoxes();
    decisionBoxGroup_->setMaximumWidth(200);
    decisionAndFormLayout->addWidget(decisionBoxGroup_);
    
    // Form-specific fields (will be added in finishSetup)
    QWidget* formFieldsWidget = new QWidget();
    formFieldsLayout_ = new QVBoxLayout(formFieldsWidget);
    formFieldsLayout_->setContentsMargins(10, 0, 0, 0);
    decisionAndFormLayout->addWidget(formFieldsWidget);
    
    rightColumnLayout->addLayout(decisionAndFormLayout);
    
    // Add action buttons to column 2 instead of bottom
    setupActionButtons();
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(freezeButton_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton_);
    buttonLayout->addWidget(printButton_);
    buttonLayout->addWidget(backButton_);
    rightColumnLayout->addLayout(buttonLayout);
    
    rightColumnLayout->addStretch();
    
    // Add the two columns with 3:4 proportions (3/7 and 4/7 of screen)
    mainContentLayout->addLayout(leftColumnLayout, 3);  // Column 1 - 3/7 of screen
    mainContentLayout->addLayout(rightColumnLayout, 4); // Column 2 - 4/7 of screen
    
    contentLayout_->addLayout(mainContentLayout);
    
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
    // Create logo label
    logoLabel_ = new QLabel();
    logoLabel_->setAlignment(Qt::AlignCenter);
    logoLabel_->setMaximumHeight(80);
    logoLabel_->setScaledContents(true);
    
    // Load and set the Nemours logo
    QPixmap logo(":/images/nemours-logo.png");
    if (!logo.isNull()) {
        logoLabel_->setPixmap(logo.scaled(200, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel_->setText("Nemours Children's Health");
        logoLabel_->setStyleSheet("font-weight: bold; color: #0066CC;");
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

void BaseFormWidget::setupSpecificationTable()
{
    specificationTable_ = new QTableWidget();
    specificationTable_->setColumnCount(7); // Reduced from 8 to 7 (removed Make/Model)
    
    QStringList headers;
    headers << "Size" << "Type" << "Cuff" << "Inner Dia." << "Outer Dia." << "Length" << "Re-order #";
    specificationTable_->setHorizontalHeaderLabels(headers);
    
    specificationTable_->setMinimumHeight(100);  // Increased height for taller row
    specificationTable_->setMaximumHeight(150); // Increased max height for taller row
    specificationTable_->setAlternatingRowColors(false); // No need for alternating colors with single row
    specificationTable_->setSelectionBehavior(QAbstractItemView::SelectItems);
    specificationTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    specificationTable_->setWordWrap(true);
    // Set minimum row height and hide vertical header (row numbers)
    specificationTable_->verticalHeader()->setMinimumSectionSize(50); // Minimum row height of 50px
    specificationTable_->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    specificationTable_->verticalHeader()->setVisible(false); // Hide row numbers since it's single-row
    
    // Set optimized column widths for the 7 columns (Make/Model removed)
    specificationTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    specificationTable_->horizontalHeader()->resizeSection(0, 100); // Size - small
    specificationTable_->horizontalHeader()->resizeSection(1, 150); // Type - medium
    specificationTable_->horizontalHeader()->resizeSection(2, 100); // Cuff - small
    specificationTable_->horizontalHeader()->resizeSection(3, 150); // Inner Diameter - medium
    specificationTable_->horizontalHeader()->resizeSection(4, 150); // Outer Diameter - medium
    specificationTable_->horizontalHeader()->resizeSection(5, 100); // Length - small
    specificationTable_->horizontalHeader()->resizeSection(6, 120); // Re-order # - medium
    
    // Let columns stretch and wrap naturally instead of fixed widths
    
    // Apply text wrapping delegate to all columns with custom character limits
    TextWrapDelegate* delegate = new TextWrapDelegate(this);
    
    // Set character limits for each column based on content type and column width (7 columns)
    QVector<int> columnLimits = {
        5,  // Column 0: Size - short values like "6.0", "7.5"
        9,  // Column 1: Type - medium descriptions
        5,  // Column 2: Cuff - "Cuffed", "Uncuffed"
        12,  // Column 3: Inner Diameter - numeric with units
        12,  // Column 4: Outer Diameter - numeric with units  
        5,  // Column 5: Length - numeric with units
        6   // Column 6: Re-order # - part numbers/codes
    };
    delegate->setColumnCharacterLimits(columnLimits);
    
    for (int i = 0; i < 7; ++i) {
        specificationTable_->setItemDelegateForColumn(i, delegate);
    }
    
    // This connection is crucial for resizing rows when the view changes
    connect(specificationTable_->horizontalHeader(), &QHeaderView::sectionResized,
            specificationTable_, &QTableWidget::resizeRowsToContents);
    
    addSpecificationRow();
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
    decisionBoxGroup_ = new QGroupBox("Decision Boxes");
    decisionBoxGroup_->setMinimumWidth(300);
    QVBoxLayout* layout = new QVBoxLayout(decisionBoxGroup_);
    
    maskVentilateCheckBox_ = new QCheckBox("Mask Ventilate");
    intubateAboveCheckBox_ = new QCheckBox("Intubate Above");
    intubateStomaCheckBox_ = new QCheckBox("Intubate Stoma");
    
    layout->addWidget(maskVentilateCheckBox_);
    layout->addWidget(intubateAboveCheckBox_);
    layout->addWidget(intubateStomaCheckBox_);
}

void BaseFormWidget::setupSidePanel()
{
    sidePanelGroup_ = new QGroupBox("Suction & Comments");
    QVBoxLayout* layout = new QVBoxLayout(sidePanelGroup_);
    
    QHBoxLayout* suctionLayout = new QHBoxLayout();
    suctionLayout->addWidget(new QLabel("Suction Size:"));
    suctionSizeSpinBox_ = new QSpinBox();
    suctionSizeSpinBox_->setMinimumWidth(200);
    suctionSizeSpinBox_->setRange(1, 20);
    suctionSizeSpinBox_->setValue(6);
    suctionSizeSpinBox_->setAlignment(Qt::AlignLeft);

    suctionDepthEdit_ = new QLineEdit();
    suctionDepthEdit_->setAlignment(Qt::AlignLeft);
    suctionLayout->addWidget(suctionSizeSpinBox_);
    
    suctionLayout->addWidget(new QLabel("Depth:"));
    suctionDepthEdit_->setPlaceholderText("e.g., 5 cm");
    suctionLayout->addWidget(suctionDepthEdit_);
    
    layout->addLayout(suctionLayout);
    
    layout->addWidget(new QLabel("Special Comments:"));
    specialCommentsEdit_ = new QTextEdit();
    specialCommentsEdit_->setMinimumHeight(150);
    specialCommentsEdit_->setMaximumHeight(200);
    
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
    
    connect(freezeButton_, &QPushButton::toggled, this, &BaseFormWidget::onFreezeClicked);
    connect(saveButton_, &QPushButton::clicked, this, &BaseFormWidget::onSaveClicked);
    connect(printButton_, &QPushButton::clicked, this, &BaseFormWidget::onPrintClicked);
    connect(backButton_, &QPushButton::clicked, this, &BaseFormWidget::onBackClicked);
}

void BaseFormWidget::connectSignals()
{
    connect(patientInfoWidget_, &PatientInfoWidget::patientInfoChanged, this, &BaseFormWidget::onPatientInfoChanged);
    connect(emergencyPanel_, &EmergencyPanel::scenarioSelected, this, &BaseFormWidget::onEmergencyScenarioSelected);
    connect(suctionSizeSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, &BaseFormWidget::onSuctionSizeChanged);
    connect(specificationTable_, &QTableWidget::cellChanged, this, &BaseFormWidget::onSpecTableChanged);
    
    connect(maskVentilateCheckBox_, &QCheckBox::toggled, this, &BaseFormWidget::onFormFieldChanged);
    connect(intubateAboveCheckBox_, &QCheckBox::toggled, this, &BaseFormWidget::onFormFieldChanged);
    connect(intubateStomaCheckBox_, &QCheckBox::toggled, this, &BaseFormWidget::onFormFieldChanged);
    connect(suctionDepthEdit_, &QLineEdit::textChanged, this, &BaseFormWidget::onFormFieldChanged);
    connect(specialCommentsEdit_, &QTextEdit::textChanged, this, &BaseFormWidget::onFormFieldChanged);
    connect(makeModelEdit_, &QTextEdit::textChanged, this, &BaseFormWidget::onFormFieldChanged);
}

void BaseFormWidget::updateStyles()
{
    StyleManager::instance().applyFormStyle(this, caseType_);
    StyleManager::instance().applyTableStyle(specificationTable_);
}

void BaseFormWidget::setCase(const Case& case_)
{
    currentCase_ = case_;
    loadFormData();
    justSaved_ = true; // Loading a case means it's saved
}

Case BaseFormWidget::getCase() const
{
    // Update case with current table contents first
    const_cast<BaseFormWidget*>(this)->updateCaseFromTable();
    Case case_ = currentCase_;
    
    // Now update all form fields
    case_.setPatient(patientInfoWidget_->getPatientInfo());
    case_.setEmergencyScenario(emergencyPanel_->getSelectedScenario());
    
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
    emergencyPanel_->setFrozen(false);
    
    specificationTable_->setEnabled(!frozen);
    // Table manipulation buttons removed for single-row mode
    
    decisionBoxGroup_->setEnabled(!frozen);
    suctionSizeSpinBox_->setEnabled(!frozen);
    suctionDepthEdit_->setReadOnly(frozen);
    specialCommentsEdit_->setReadOnly(frozen);
    makeModelEdit_->setReadOnly(frozen);
    
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
    
    if (specificationTable_->rowCount() == 0) {
        errors << "At least one specification row is required";
    }
    
    return errors;
}

void BaseFormWidget::clear()
{
    patientInfoWidget_->clear();
    emergencyPanel_->clear();
    
    specificationTable_->setRowCount(0);
    addSpecificationRow(); // Only add one row for single-row mode
    
    maskVentilateCheckBox_->setChecked(false);
    intubateAboveCheckBox_->setChecked(false);
    intubateStomaCheckBox_->setChecked(false);
    
    suctionSizeSpinBox_->setValue(6);
    suctionDepthEdit_->clear();
    specialCommentsEdit_->clear();
    makeModelEdit_->clear();
    
    currentCase_ = Case(QString(), caseType_);
    justSaved_ = true; // New empty case is considered "saved"
}

void BaseFormWidget::addSpecificationRow()
{
    int row = specificationTable_->rowCount();
    specificationTable_->insertRow(row);
    
    for (int col = 0; col < 7; ++col) { // Changed from 8 to 7 columns
        QTableWidgetItem* item = new QTableWidgetItem();
        specificationTable_->setItem(row, col, item);
    }
    
    specificationTable_->resizeRowsToContents();
    onSpecTableChanged();
}

void BaseFormWidget::removeSpecificationRow()
{
    int currentRow = specificationTable_->currentRow();
    if (currentRow >= 0) {
        specificationTable_->removeRow(currentRow);
        onSpecTableChanged();
    }
}

void BaseFormWidget::moveSpecificationRowUp()
{
    int currentRow = specificationTable_->currentRow();
    if (currentRow > 0) {
        specificationTable_->insertRow(currentRow - 1);
        
        for (int col = 0; col < specificationTable_->columnCount(); ++col) {
            QTableWidgetItem* item = specificationTable_->takeItem(currentRow + 1, col);
            specificationTable_->setItem(currentRow - 1, col, item);
        }
        
        specificationTable_->removeRow(currentRow + 1);
        specificationTable_->setCurrentCell(currentRow - 1, 0);
        onSpecTableChanged();
    }
}

void BaseFormWidget::moveSpecificationRowDown()
{
    int currentRow = specificationTable_->currentRow();
    if (currentRow >= 0 && currentRow < specificationTable_->rowCount() - 1) {
        specificationTable_->insertRow(currentRow + 2);
        
        for (int col = 0; col < specificationTable_->columnCount(); ++col) {
            QTableWidgetItem* item = specificationTable_->takeItem(currentRow, col);
            specificationTable_->setItem(currentRow + 2, col, item);
        }
        
        specificationTable_->removeRow(currentRow);
        specificationTable_->setCurrentCell(currentRow + 1, 0);
        onSpecTableChanged();
    }
}

void BaseFormWidget::loadFormData()
{
    patientInfoWidget_->setPatientInfo(currentCase_.getPatient());
    emergencyPanel_->setSelectedScenario(currentCase_.getEmergencyScenario());
    
    updateTableFromCase();
    
    SuctionInfo suction = currentCase_.getSuction();
    suctionSizeSpinBox_->setValue(suction.size);
    suctionDepthEdit_->setText(suction.depth);
    
    DecisionBox decision = currentCase_.getDecisionBox();
    maskVentilateCheckBox_->setChecked(decision.maskVentilate);
    intubateAboveCheckBox_->setChecked(decision.intubateAbove);
    intubateStomaCheckBox_->setChecked(decision.intubateStoma);
    
    specialCommentsEdit_->setPlainText(currentCase_.getSpecialComments());
    
    // After loading, tell the table to resize all rows based on the delegate's hints
    specificationTable_->resizeRowsToContents();
}

void BaseFormWidget::updateTableFromCase()
{
    QList<SpecificationTableRow> specs = currentCase_.getSpecTable();
    
    // Clear existing table content
    specificationTable_->setRowCount(0);
    
    // Ensure we always have exactly one row for the single-row table
    if (specs.isEmpty()) {
        addSpecificationRow();
        return;
    }
    
    // For single-row mode, only use the first specification
    if (specs.size() > 1) {
        specs = QList<SpecificationTableRow>() << specs.first();
    }
    
    // Set row count and populate data
    specificationTable_->setRowCount(specs.size());
    
    for (int row = 0; row < specs.size(); ++row) {
        const SpecificationTableRow& spec = specs[row];
        
        // Make/Model is now handled separately, table columns shifted down
        specificationTable_->setItem(row, 0, new QTableWidgetItem(spec.size));
        specificationTable_->setItem(row, 1, new QTableWidgetItem(spec.type));
        specificationTable_->setItem(row, 2, new QTableWidgetItem(spec.cuff));
        specificationTable_->setItem(row, 3, new QTableWidgetItem(spec.innerDiameter == 0.0 ? "" : QString::number(spec.innerDiameter)));
        specificationTable_->setItem(row, 4, new QTableWidgetItem(spec.outerDiameter == 0.0 ? "" : QString::number(spec.outerDiameter)));
        specificationTable_->setItem(row, 5, new QTableWidgetItem(spec.length == 0.0 ? "" : QString::number(spec.length)));
        specificationTable_->setItem(row, 6, new QTableWidgetItem(spec.reorderNumber == 0 ? "" : QString::number(spec.reorderNumber)));
        
        // Set Make/Model in the separate text area
        if (makeModelEdit_) {
            makeModelEdit_->setPlainText(spec.makeModel);
        }
    }
}

void BaseFormWidget::updateCaseFromTable()
{
    QList<SpecificationTableRow> specs;
    
    for (int row = 0; row < specificationTable_->rowCount(); ++row) {
        SpecificationTableRow spec;
        
        // Get Make/Model from the separate text area
        spec.makeModel = makeModelEdit_ ? makeModelEdit_->toPlainText() : "";
        
        // Get other fields from table (columns shifted due to Make/Model removal)
        QTableWidgetItem* item = specificationTable_->item(row, 0);
        spec.size = item ? item->text() : "";
        
        item = specificationTable_->item(row, 1);
        spec.type = item ? item->text() : "";
        
        item = specificationTable_->item(row, 2);
        spec.cuff = item ? item->text() : "";
        
        item = specificationTable_->item(row, 3);
        spec.innerDiameter = item ? item->text().toDouble() : 0.0;
        
        item = specificationTable_->item(row, 4);
        spec.outerDiameter = item ? item->text().toDouble() : 0.0;
        
        item = specificationTable_->item(row, 5);
        spec.length = item ? item->text().toDouble() : 0.0;
        
        item = specificationTable_->item(row, 6);
        spec.reorderNumber = item ? item->text().toInt() : 0;
        
        specs.append(spec);
    }
    
    currentCase_.setSpecTable(specs);
}

void BaseFormWidget::onSaveClicked()
{
    if (!isFormValid()) {
        QStringList errors = getValidationErrors();
        QMessageBox::warning(this, "Validation Error", 
                           "Please fix the following errors:\n\n" + errors.join("\n"));
        return;
    }
    
    // Update case data from current table contents before saving
    updateCaseFromTable();
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
    emergencyPanel_->setSuctionSize(size);
    emit formChanged();
}

void BaseFormWidget::onSpecTableChanged()
{
    justSaved_ = false;
    // Resize rows when content changes to accommodate wrapped text
    specificationTable_->resizeRowsToContents();
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