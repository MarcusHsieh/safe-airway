#include "EmergencyPanelOverlay.h"
#include "utils/StyleManager.h"
#include "models/EmergencyScenario.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

EmergencyPanelOverlay::EmergencyPanelOverlay(QWidget* parent)
    : QWidget(parent)
    , overlayPanel_(nullptr)
    , panelLayout_(nullptr)
    , titleLabel_(nullptr)
    , scenarioGroupBox_(nullptr)
    , scenarioGroup_(nullptr)
    , cantSuctionButton_(nullptr)
    , cantVentilateButton_(nullptr)
    , o2SatDropButton_(nullptr)
    , decannulationButton_(nullptr)
    , hemoptysisButton_(nullptr)
    , instructionsEdit_(nullptr)
    , closeButton_(nullptr)
    , suctionSize_(6)
    , visible_(false)
{
    setupUI();
    setupButtonStyling();
    connectSignals();
    
    // Initially hidden
    hide();
    
    // Accept focus to receive key events
    setFocusPolicy(Qt::StrongFocus);
}

void EmergencyPanelOverlay::setupUI()
{
    // Full screen overlay
    setGeometry(0, 0, QGuiApplication::primaryScreen()->availableSize().width(),
                QGuiApplication::primaryScreen()->availableSize().height());
    
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Center the overlay panel
    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    
    // Overlay panel with modern card styling
    overlayPanel_ = new QWidget();
    overlayPanel_->setFixedSize(1300, 800);
    overlayPanel_->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 12px;"
        "}"
    );

    panelLayout_ = new QVBoxLayout(overlayPanel_);
    panelLayout_->setSpacing(25);
    panelLayout_->setContentsMargins(40, 35, 40, 35);

    // Title
    titleLabel_ = new QLabel("Emergency Scenarios");
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setFont(StyleManager::instance().getEmergencyTitleFont());
    titleLabel_->setStyleSheet("color: #DC143C; font-weight: bold;");
    panelLayout_->addWidget(titleLabel_);
    
    // Create horizontal layout for scenarios and instructions side by side
    QHBoxLayout* contentLayout = new QHBoxLayout();
    
    // Left side: Emergency scenarios
    QVBoxLayout* leftColumnLayout = new QVBoxLayout();
    
    scenarioGroupBox_ = new QGroupBox("Select Type:");
    scenarioGroupBox_->setMinimumHeight(200);
    scenarioGroupBox_->setMaximumWidth(450);
    scenarioGroupBox_->setMinimumWidth(400);
    scenarioGroupBox_->setStyleSheet(
        "QGroupBox {"
        "   background-color: #FAFAFA;"
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
        "   background-color: #FAFAFA;"
        "}"
    );
    QVBoxLayout* scenarioLayout = new QVBoxLayout(scenarioGroupBox_);
    scenarioLayout->setSpacing(12);
    scenarioLayout->setContentsMargins(15, 25, 15, 15);
    
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
    
    // Set minimum height and width for better visibility
    cantSuctionButton_->setMinimumHeight(60);
    cantVentilateButton_->setMinimumHeight(60);
    o2SatDropButton_->setMinimumHeight(60);
    decannulationButton_->setMinimumHeight(60);
    hemoptysisButton_->setMinimumHeight(60);

    cantSuctionButton_->setMinimumWidth(350);
    cantVentilateButton_->setMinimumWidth(350);
    o2SatDropButton_->setMinimumWidth(350);
    decannulationButton_->setMinimumWidth(350);
    hemoptysisButton_->setMinimumWidth(350);
    
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
    instructionsLabel->setStyleSheet("color: #2C3E50; font-weight: bold;");
    instructionsLayout->addWidget(instructionsLabel);
    
    instructionsEdit_ = new QTextEdit();
    instructionsEdit_->setReadOnly(true);
    instructionsEdit_->setMinimumHeight(500);
    instructionsEdit_->setMaximumHeight(650);
    instructionsEdit_->setMinimumWidth(600);
    instructionsEdit_->setWordWrapMode(QTextOption::WordWrap);
    instructionsEdit_->setPlaceholderText("Select an emergency scenario to view instructions.");
    instructionsEdit_->setFont(StyleManager::instance().getEmergencyInstructionsFont());
    instructionsEdit_->setStyleSheet(
        "QTextEdit {"
        "   background-color: #FAFAFA;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "}"
    );
    instructionsLayout->addWidget(instructionsEdit_);
    
    contentLayout->addLayout(instructionsLayout);
    
    panelLayout_->addLayout(contentLayout);
    
    // Close button
    closeButton_ = new QPushButton("Close");
    closeButton_->setMinimumHeight(50);
    closeButton_->setMinimumWidth(120);
    closeButton_->setCursor(Qt::PointingHandCursor);

    QHBoxLayout* closeLayout = new QHBoxLayout();
    closeLayout->addStretch();
    closeLayout->addWidget(closeButton_);

    panelLayout_->addLayout(closeLayout);
    
    centerLayout->addWidget(overlayPanel_);
    centerLayout->addStretch();
    
    mainLayout->addStretch();
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();
}

void EmergencyPanelOverlay::setupButtonStyling()
{
    // Style emergency scenario buttons with different colors
    cantSuctionButton_->setStyleSheet(getButtonStyleSheet("#DC143C")); // Red
    cantVentilateButton_->setStyleSheet(getButtonStyleSheet("#FF4500")); // Orange Red
    o2SatDropButton_->setStyleSheet(getButtonStyleSheet("#FF8C00")); // Dark Orange
    decannulationButton_->setStyleSheet(getButtonStyleSheet("#B22222")); // Fire Brick
    hemoptysisButton_->setStyleSheet(getButtonStyleSheet("#8B0000")); // Dark Red
    
    // Close button style
    closeButton_->setStyleSheet(getButtonStyleSheet("#666666"));
}

QString EmergencyPanelOverlay::getButtonStyleSheet(const QString& color) const
{
    // Calculate darker hover color (darken by 15%)
    QColor baseColor(color);
    QColor hoverColor = baseColor.darker(115);

    return QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 26px;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: %2;"
        "   padding-top: 14px;"
        "   padding-bottom: 10px;"
        "}"
        "QPushButton:checked {"
        "   background-color: %1;"
        "   border: 4px solid #FFD700;"
        "   color: white;"
        "   font-weight: bold;"
        "}"
    ).arg(color, hoverColor.name());
}

void EmergencyPanelOverlay::connectSignals()
{
    connect(scenarioGroup_, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), 
            this, &EmergencyPanelOverlay::onScenarioButtonClicked);
    connect(closeButton_, &QPushButton::clicked, this, &EmergencyPanelOverlay::hideOverlay);
}

void EmergencyPanelOverlay::showOverlay()
{
    visible_ = true;
    show();
    raise();
    setFocus();
    
    // Ensure we're on top and cover the entire window
    if (parentWidget()) {
        setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
    }
}

void EmergencyPanelOverlay::hideOverlay()
{
    visible_ = false;
    hide();
    
    // Return focus to parent
    if (parentWidget()) {
        parentWidget()->setFocus();
    }
}

bool EmergencyPanelOverlay::isOverlayVisible() const
{
    return visible_;
}

void EmergencyPanelOverlay::setSelectedScenario(const QString& scenarioName)
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

QString EmergencyPanelOverlay::getSelectedScenario() const
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

void EmergencyPanelOverlay::setSuctionSize(int size)
{
    suctionSize_ = size;
    updateInstructions();
}

void EmergencyPanelOverlay::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    
    // Draw semi-transparent background
    painter.fillRect(rect(), QColor(0, 0, 0, 150));
    
    QWidget::paintEvent(event);
}

void EmergencyPanelOverlay::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        hideOverlay();
        event->accept();
        return;
    }
    
    QWidget::keyPressEvent(event);
}

void EmergencyPanelOverlay::mousePressEvent(QMouseEvent* event)
{
    // Close overlay if clicking outside the overlay panel
    if (!overlayPanel_->geometry().contains(event->pos())) {
        hideOverlay();
        event->accept();
        return;
    }
    
    QWidget::mousePressEvent(event);
}

void EmergencyPanelOverlay::onScenarioButtonClicked()
{
    QString selectedScenario = getSelectedScenario();
    emit scenarioSelected(selectedScenario);
    updateInstructions();
}

void EmergencyPanelOverlay::updateInstructions()
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

QString EmergencyPanelOverlay::getInstructionsWithETTSize(const QString& instructions) const
{
    QString modifiedInstructions = instructions;
    
    QString ettSize = QString::number(suctionSize_) + ".0";
    modifiedInstructions.replace("6.0 endotracheal tube", ettSize + " endotracheal tube");
    
    return modifiedInstructions;
}