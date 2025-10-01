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
    , loading_(false)
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
    // Set modern background color
    setStyleSheet("QWidget { background-color: #F5F5F5; }");

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0);
    mainLayout_->setSpacing(0);

    scrollArea_ = new QScrollArea();
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea_->setStyleSheet("QScrollArea { border: none; background-color: #F5F5F5; }");

    contentWidget_ = new QWidget();
    contentWidget_->setStyleSheet("QWidget { background-color: #F5F5F5; }");
    contentLayout_ = new QVBoxLayout(contentWidget_);

    contentLayout_->setSpacing(20);
    contentLayout_->setContentsMargins(25, 20, 25, 20);
    
    setupHeader();

    // Get case type color for branding
    QColor caseColor = StyleManager::instance().getFormColour(caseType_);

    // Modern header card with colored accent
    QWidget* headerWidget = new QWidget();
    QVBoxLayout* headerMainLayout = new QVBoxLayout(headerWidget);
    headerMainLayout->setContentsMargins(0, 0, 0, 0);
    headerMainLayout->setSpacing(0);

    // Colored accent bar at top
    QWidget* accentBar = new QWidget();
    accentBar->setFixedHeight(6);
    accentBar->setStyleSheet(QString("background-color: %1; border-top-left-radius: 12px; border-top-right-radius: 12px;").arg(caseColor.name()));
    headerMainLayout->addWidget(accentBar);

    // Header content
    QWidget* headerContent = new QWidget();
    headerContent->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border-bottom-left-radius: 12px;"
        "   border-bottom-right-radius: 12px;"
        "}"
    );
    QHBoxLayout* headerLayout = new QHBoxLayout(headerContent);
    headerLayout->setSpacing(20);
    headerLayout->setContentsMargins(20, 15, 20, 15);

    // Add logo to the left
    headerLayout->addWidget(logoLabel_);

    // Add patient info widget to header
    patientInfoWidget_ = new PatientInfoWidget();
    headerLayout->addWidget(patientInfoWidget_);

    // Add form title with case type color
    headerLabel_->setStyleSheet(QString("font-size: 32px; font-weight: bold; color: %1;").arg(caseColor.name()));
    headerLayout->addWidget(headerLabel_, 1);

    headerMainLayout->addWidget(headerContent);

    // Apply outer border to entire header widget
    headerWidget->setStyleSheet(QString(
        "QWidget#headerCard {"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 12px;"
        "   border-left: 4px solid %1;"
        "}"
    ).arg(caseColor.name()));
    headerWidget->setObjectName("headerCard");

    contentLayout_->addWidget(headerWidget);
    
    // Create two main columns layout
    QHBoxLayout* mainContentLayout = new QHBoxLayout();
    
    // COLUMN 1: Suction & Comments and Form-specific fields
    QVBoxLayout* leftColumnLayout = new QVBoxLayout();

    // Suction & Comments panel
    setupSidePanel();
    leftColumnLayout->addWidget(sidePanelGroup_);

    // Form-specific fields (Tracheostomy Information, etc.)
    QWidget* formFieldsWidget = new QWidget();
    formFieldsLayout_ = new QVBoxLayout(formFieldsWidget);
    formFieldsLayout_->setContentsMargins(0, 10, 0, 0);
    leftColumnLayout->addWidget(formFieldsWidget);

    // Create emergency panel overlay (initially hidden)
    emergencyPanelOverlay_ = new EmergencyPanelOverlay(this);

    leftColumnLayout->addStretch();

    // COLUMN 2: Tube Specification Widget
    QVBoxLayout* rightColumnLayout = new QVBoxLayout();

    // Tube specification widget (full column width)
    tubeSpecWidget_ = new TubeSpecificationWidget();
    rightColumnLayout->addWidget(tubeSpecWidget_);
    
    rightColumnLayout->addStretch();
    
    // Add the two columns with 30:70 split
    mainContentLayout->addLayout(leftColumnLayout, 30);  // Column 1 - 30% of screen
    mainContentLayout->addLayout(rightColumnLayout, 70); // Column 2 - 70% of screen
    
    contentLayout_->addLayout(mainContentLayout);
    
    // Add bottom action buttons bar
    setupActionButtons();

    // Bottom button bar with card styling
    QWidget* buttonBarWidget = new QWidget();
    buttonBarWidget->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border-radius: 12px;"
        "   border: 1px solid #E0E0E0;"
        "}"
    );
    QHBoxLayout* bottomButtonLayout = new QHBoxLayout(buttonBarWidget);
    bottomButtonLayout->setSpacing(12);
    bottomButtonLayout->setContentsMargins(20, 15, 20, 15);

    // Emergency scenarios button (moved from Column 1)
    emergencyButton_ = new QPushButton("Emergency Scenarios");
    emergencyButton_->setMinimumHeight(50);
    emergencyButton_->setCursor(Qt::PointingHandCursor);
    emergencyButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #DC143C;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 24px;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 24px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #B22222;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #8B0000;"
        "   padding-top: 14px;"
        "   padding-bottom: 10px;"
        "}"
    );

    bottomButtonLayout->addWidget(freezeButton_);
    bottomButtonLayout->addWidget(emergencyButton_);
    bottomButtonLayout->addStretch();
    bottomButtonLayout->addWidget(saveButton_);
    bottomButtonLayout->addWidget(printButton_);
    bottomButtonLayout->addWidget(backButton_);

    contentLayout_->addWidget(buttonBarWidget);
    
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


void BaseFormWidget::setupSidePanel()
{
    sidePanelGroup_ = new QGroupBox("Suction & Comments");
    sidePanelGroup_->setFont(StyleManager::instance().getGroupBoxFont());
    sidePanelGroup_->setStyleSheet(
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
    QVBoxLayout* layout = new QVBoxLayout(sidePanelGroup_);
    layout->setSpacing(12);
    layout->setContentsMargins(15, 25, 15, 15);

    // Suction Size - vertical layout
    QLabel* suctionSizeLabel = new QLabel("Suction Size:");
    suctionSizeLabel->setFont(StyleManager::instance().getKeyElementFont());
    suctionSizeLabel->setStyleSheet("color: #546E7A; font-size: 32px; font-weight: normal;");
    layout->addWidget(suctionSizeLabel);

    suctionSizeSpinBox_ = new QSpinBox();
    suctionSizeSpinBox_->setMinimumHeight(40);
    suctionSizeSpinBox_->setRange(1, 20);
    suctionSizeSpinBox_->setValue(6);
    suctionSizeSpinBox_->setAlignment(Qt::AlignLeft);
    suctionSizeSpinBox_->setFont(StyleManager::instance().getKeyElementFont());
    suctionSizeSpinBox_->setStyleSheet(
        "QSpinBox {"
        "   background-color: #FAFAFA;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "   font-size: 32px;"
        "}"
        "QSpinBox:focus {"
        "   border: 2px solid #1976D2;"
        "   background-color: white;"
        "}"
    );
    layout->addWidget(suctionSizeSpinBox_);

    // Suction Depth - vertical layout
    QLabel* suctionDepthLabel = new QLabel("Depth:");
    suctionDepthLabel->setFont(StyleManager::instance().getKeyElementFont());
    suctionDepthLabel->setStyleSheet("color: #546E7A; font-size: 32px; font-weight: normal;");
    layout->addWidget(suctionDepthLabel);

    suctionDepthEdit_ = new QLineEdit();
    suctionDepthEdit_->setAlignment(Qt::AlignLeft);
    suctionDepthEdit_->setPlaceholderText("e.g., 5 cm");
    suctionDepthEdit_->setMinimumHeight(40);
    suctionDepthEdit_->setStyleSheet(
        "QLineEdit {"
        "   background-color: #FAFAFA;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "   font-size: 32px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #1976D2;"
        "   background-color: white;"
        "}"
    );
    layout->addWidget(suctionDepthEdit_);

    QLabel* commentsLabel = new QLabel("Special Comments:");
    commentsLabel->setStyleSheet("color: #546E7A; font-size: 32px; font-weight: normal;");
    layout->addWidget(commentsLabel);

    specialCommentsEdit_ = new QTextEdit();
    specialCommentsEdit_->setMinimumHeight(120);
    specialCommentsEdit_->setMaximumHeight(180);
    specialCommentsEdit_->setStyleSheet(
        "QTextEdit {"
        "   background-color: #FAFAFA;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 6px;"
        "   padding: 10px;"
        "   font-size: 32px;"
        "}"
        "QTextEdit:focus {"
        "   border: 2px solid #1976D2;"
        "   background-color: white;"
        "}"
    );
    
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
    auto createButton = [](const QString& text, const QString& color, const QString& hoverColor, bool checkable = false) -> QPushButton* {
        QPushButton* btn = new QPushButton(text);
        btn->setMinimumHeight(45);
        btn->setMinimumWidth(100);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setCheckable(checkable);

        QString style = QString(
            "QPushButton {"
            "   background-color: %1;"
            "   color: white;"
            "   font-weight: bold;"
            "   font-size: 24px;"
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
        ).arg(color, hoverColor);

        if (checkable) {
            style += QString(
                "QPushButton:checked {"
                "   background-color: #DC143C;"
                "   color: white;"
                "}"
            );
        }

        btn->setStyleSheet(style);
        return btn;
    };

    freezeButton_ = createButton("Freeze", "#607D8B", "#546E7A", true);
    saveButton_ = createButton("Save Case", "#1976D2", "#1565C0");
    printButton_ = createButton("Print", "#78909C", "#607D8B");
    backButton_ = createButton("Back", "#90A4AE", "#78909C");

    connect(freezeButton_, &QPushButton::toggled, this, &BaseFormWidget::onFreezeClicked);
    connect(saveButton_, &QPushButton::clicked, this, &BaseFormWidget::onSaveClicked);
    connect(printButton_, &QPushButton::clicked, this, &BaseFormWidget::onPrintClicked);
    connect(backButton_, &QPushButton::clicked, this, &BaseFormWidget::onBackClicked);
}

void BaseFormWidget::connectSignals()
{
    connect(patientInfoWidget_, &PatientInfoWidget::patientInfoChanged, this, &BaseFormWidget::onPatientInfoChanged);
    // Emergency button connection will be added after setupUI is complete
    connect(emergencyPanelOverlay_, &EmergencyPanelOverlay::scenarioSelected, this, &BaseFormWidget::onEmergencyScenarioSelected);
    connect(suctionSizeSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, &BaseFormWidget::onSuctionSizeChanged);
    connect(tubeSpecWidget_, &TubeSpecificationWidget::specificationChanged, this, &BaseFormWidget::onFormFieldChanged);

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
        freezeButton_->setText("Freeze");
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

    suctionSizeSpinBox_->setValue(6);
    suctionDepthEdit_->clear();
    specialCommentsEdit_->clear();
    
    currentCase_ = Case();  // Use default constructor to generate UUID
    currentCase_.setCaseType(caseType_);
    justSaved_ = true; // New empty case is considered "saved"
}





void BaseFormWidget::loadFormData()
{
    loading_ = true; // Prevent signals from marking form as modified

    patientInfoWidget_->setPatientInfo(currentCase_.getPatient());
    emergencyPanelOverlay_->setSelectedScenario(currentCase_.getEmergencyScenario());

    updateTableFromCase();

    SuctionInfo suction = currentCase_.getSuction();
    suctionSizeSpinBox_->setValue(suction.size);
    suctionDepthEdit_->setText(suction.depth);

    specialCommentsEdit_->setPlainText(currentCase_.getSpecialComments());

    loading_ = false; // Re-enable modification tracking
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

void BaseFormWidget::onPatientInfoChanged()
{
    if (!loading_) {
        justSaved_ = false;
        emit formChanged();
    }
}

void BaseFormWidget::onEmergencyScenarioSelected(const QString& scenario)
{
    updateEmergencyAdvice();
    emit emergencyScenarioChanged(scenario);
    if (!loading_) {
        emit formChanged();
    }
}

void BaseFormWidget::onSuctionSizeChanged(int size)
{
    emergencyPanelOverlay_->setSuctionSize(size);
    if (!loading_) {
        emit formChanged();
    }
}


void BaseFormWidget::onFormFieldChanged()
{
    if (!loading_) {
        justSaved_ = false;
        currentCase_.setUpdatedAt(QDateTime::currentDateTime());
        emit formChanged();
    }
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