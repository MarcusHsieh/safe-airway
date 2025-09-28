#include "EmergencyPanel.h"
#include "utils/StyleManager.h"

EmergencyPanel::EmergencyPanel(QWidget* parent)
    : QWidget(parent)
    , scenarioGroup_(nullptr)
    , cantSuctionButton_(nullptr)
    , cantVentilateButton_(nullptr)
    , o2SatDropButton_(nullptr)
    , decannulationButton_(nullptr)
    , hemoptysisButton_(nullptr)
    , instructionsEdit_(nullptr)
    , titleLabel_(nullptr)
    , scenarioGroupBox_(nullptr)
    , suctionSize_(6)
    , frozen_(false)
{
    setupUI();
    connectSignals();
    updateStyles();
}

void EmergencyPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create horizontal layout for scenario selection and instructions side by side
    QHBoxLayout* contentLayout = new QHBoxLayout();
    
    // Left side: Emergency scenarios column
    QVBoxLayout* leftColumnLayout = new QVBoxLayout();
    
    titleLabel_ = new QLabel("EMERGENCY\nSCENARIOS");
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setObjectName("emergencyTitle");
    titleLabel_->setWordWrap(true);
    leftColumnLayout->addWidget(titleLabel_);
    
    scenarioGroupBox_ = new QGroupBox("Select Type:");
    scenarioGroupBox_->setMinimumHeight(150);
    scenarioGroupBox_->setMaximumWidth(250);
    scenarioGroupBox_->setMinimumWidth(200);
    QVBoxLayout* scenarioLayout = new QVBoxLayout(scenarioGroupBox_);
    scenarioLayout->setSpacing(5);
    scenarioLayout->setContentsMargins(10, 15, 10, 10);
    
    scenarioGroup_ = new QButtonGroup(this);
    
    cantSuctionButton_ = new QPushButton("Can't Suction");
    cantVentilateButton_ = new QPushButton("Can't Ventilate");
    o2SatDropButton_ = new QPushButton("O2 Sat Drop");
    decannulationButton_ = new QPushButton("Decannulation");
    hemoptysisButton_ = new QPushButton("Hemoptysis");
    
    // Make buttons checkable so they can be toggled
    cantSuctionButton_->setCheckable(true);
    cantVentilateButton_->setCheckable(true);
    o2SatDropButton_->setCheckable(true);
    decannulationButton_->setCheckable(true);
    hemoptysisButton_->setCheckable(true);
    
    // Set minimum height for better visibility
    cantSuctionButton_->setMinimumHeight(40);
    cantVentilateButton_->setMinimumHeight(40);
    o2SatDropButton_->setMinimumHeight(40);
    decannulationButton_->setMinimumHeight(40);
    hemoptysisButton_->setMinimumHeight(40);
    
    scenarioGroup_->addButton(cantSuctionButton_, 0);
    scenarioGroup_->addButton(cantVentilateButton_, 1);
    scenarioGroup_->addButton(o2SatDropButton_, 2);
    scenarioGroup_->addButton(decannulationButton_, 3);
    scenarioGroup_->addButton(hemoptysisButton_, 4);
    
    // Make button group exclusive (only one can be selected at a time)
    scenarioGroup_->setExclusive(true);
    
    scenarioLayout->addWidget(cantSuctionButton_);
    scenarioLayout->addWidget(cantVentilateButton_);
    scenarioLayout->addWidget(o2SatDropButton_);
    scenarioLayout->addWidget(decannulationButton_);
    scenarioLayout->addWidget(hemoptysisButton_);
    scenarioLayout->addStretch();
    
    leftColumnLayout->addWidget(scenarioGroupBox_);
    leftColumnLayout->addStretch();
    
    contentLayout->addLayout(leftColumnLayout);
    
    // Right side: Emergency instructions
    QVBoxLayout* instructionsLayout = new QVBoxLayout();
    QLabel* instructionsLabel = new QLabel("Emergency Instructions:");
    instructionsLabel->setFont(StyleManager::instance().getEmergencyLabelFont());
    instructionsLayout->addWidget(instructionsLabel);
    
    instructionsEdit_ = new QTextEdit();
    instructionsEdit_->setReadOnly(true);
    instructionsEdit_->setMinimumHeight(450);  // Increased height for more space
    instructionsEdit_->setMaximumHeight(500);  // Increased max height
    instructionsEdit_->setMinimumWidth(400);   // Increased width for better readability
    instructionsEdit_->setWordWrapMode(QTextOption::WordWrap);
    instructionsEdit_->setPlaceholderText("Select an emergency scenario to view instructions.");
    instructionsLayout->addWidget(instructionsEdit_);
    
    contentLayout->addLayout(instructionsLayout);
    
    mainLayout->addLayout(contentLayout);
    
    setLayout(mainLayout);
}

void EmergencyPanel::connectSignals()
{
    connect(scenarioGroup_, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &EmergencyPanel::onScenarioButtonClicked);
}

void EmergencyPanel::updateStyles()
{
    StyleManager::instance().applyEmergencyStyle(this);
    
    // Apply specific fonts for each emergency panel element
    titleLabel_->setFont(StyleManager::instance().getEmergencyTitleFont());
    
    // Apply emergency radio font to the push buttons
    cantSuctionButton_->setFont(StyleManager::instance().getEmergencyRadioFont());
    cantVentilateButton_->setFont(StyleManager::instance().getEmergencyRadioFont());
    o2SatDropButton_->setFont(StyleManager::instance().getEmergencyRadioFont());
    decannulationButton_->setFont(StyleManager::instance().getEmergencyRadioFont());
    hemoptysisButton_->setFont(StyleManager::instance().getEmergencyRadioFont());
    
    instructionsEdit_->setFont(StyleManager::instance().getEmergencyInstructionsFont());
}

void EmergencyPanel::setSelectedScenario(const QString& scenarioName)
{
    // Clear all selections first
    scenarioGroup_->setExclusive(false);
    cantSuctionButton_->setChecked(false);
    cantVentilateButton_->setChecked(false);
    o2SatDropButton_->setChecked(false);
    decannulationButton_->setChecked(false);
    hemoptysisButton_->setChecked(false);
    scenarioGroup_->setExclusive(true);
    
    // Set the selected scenario
    if (scenarioName == "can't suction") {
        cantSuctionButton_->setChecked(true);
    } else if (scenarioName == "can't ventilate") {
        cantVentilateButton_->setChecked(true);
    } else if (scenarioName == "o2 sat drop") {
        o2SatDropButton_->setChecked(true);
    } else if (scenarioName == "decannulation") {
        decannulationButton_->setChecked(true);
    } else if (scenarioName == "hemoptysis") {
        hemoptysisButton_->setChecked(true);
    }
    
    updateInstructions();
}

QString EmergencyPanel::getSelectedScenario() const
{
    if (cantSuctionButton_->isChecked()) {
        return "can't suction";
    } else if (cantVentilateButton_->isChecked()) {
        return "can't ventilate";
    } else if (o2SatDropButton_->isChecked()) {
        return "o2 sat drop";
    } else if (decannulationButton_->isChecked()) {
        return "decannulation";
    } else if (hemoptysisButton_->isChecked()) {
        return "hemoptysis";
    }
    
    return QString();
}

void EmergencyPanel::setSuctionSize(int size)
{
    suctionSize_ = size;
    updateInstructions();
}

void EmergencyPanel::setFrozen(bool frozen)
{
    frozen_ = frozen;
    
    scenarioGroupBox_->setEnabled(!frozen);
    
    if (frozen) {
        StyleManager::instance().applyFreezeOverlay(scenarioGroupBox_, true);
    } else {
        StyleManager::instance().applyFreezeOverlay(scenarioGroupBox_, false);
        updateStyles();
    }
}

void EmergencyPanel::clear()
{
    scenarioGroup_->setExclusive(false);
    cantSuctionButton_->setChecked(false);
    cantVentilateButton_->setChecked(false);
    o2SatDropButton_->setChecked(false);
    decannulationButton_->setChecked(false);
    hemoptysisButton_->setChecked(false);
    scenarioGroup_->setExclusive(true);
    
    instructionsEdit_->clear();
}

void EmergencyPanel::onScenarioButtonClicked()
{
    QString selectedScenario = getSelectedScenario();
    emit scenarioSelected(selectedScenario);
    updateInstructions();
}

void EmergencyPanel::updateInstructions()
{
    QString selectedScenario = getSelectedScenario();
    
    if (selectedScenario.isEmpty()) {
        instructionsEdit_->clear();
        return;
    }
    
    EmergencyScenario scenario = EmergencyScenario::getScenarioByName(selectedScenario);
    QString instructions = scenario.getFormattedInstructions();
    
    if (selectedScenario == "can't ventilate" || selectedScenario == "decannulation") {
        instructions = getInstructionsWithETTSize(instructions);
    }
    
    instructionsEdit_->setText(instructions);
    emit instructionsChanged(instructions);
}

QString EmergencyPanel::getInstructionsWithETTSize(const QString& instructions) const
{
    QString modifiedInstructions = instructions;
    
    QString ettSize = QString::number(suctionSize_) + ".0";
    modifiedInstructions.replace("6.0 endotracheal tube", ettSize + " endotracheal tube");
    
    return modifiedInstructions;
}