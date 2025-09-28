#include "EscOverlayMenu.h"
#include "utils/StyleManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

EscOverlayMenu::EscOverlayMenu(QWidget* parent)
    : QWidget(parent)
    , menuPanel_(nullptr)
    , buttonLayout_(nullptr)
    , newCaseButton_(nullptr)
    , openCaseButton_(nullptr)
    , saveButton_(nullptr)
    , saveAsButton_(nullptr)
    , increaseFontButton_(nullptr)
    , decreaseFontButton_(nullptr)
    , resetFontButton_(nullptr)
    , aboutButton_(nullptr)
    , exitButton_(nullptr)
    , titleLabel_(nullptr)
    , visible_(false)
{
    setupUI();
    setupButtonStyling();
    
    // Initially hidden
    hide();
    
    // Accept focus to receive key events
    setFocusPolicy(Qt::StrongFocus);
}

void EscOverlayMenu::setupUI()
{
    // Full screen overlay
    setGeometry(0, 0, QGuiApplication::primaryScreen()->availableSize().width(),
                QGuiApplication::primaryScreen()->availableSize().height());
    
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Center the menu panel
    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    
    // Menu panel with background
    menuPanel_ = new QWidget();
    menuPanel_->setFixedSize(600, 500);
    menuPanel_->setStyleSheet(
        "QWidget {"
        "   background-color: rgba(255, 255, 255, 240);"
        "   border: 2px solid #CCCCCC;"
        "   border-radius: 15px;"
        "}"
    );
    
    QVBoxLayout* panelLayout = new QVBoxLayout(menuPanel_);
    panelLayout->setSpacing(20);
    panelLayout->setContentsMargins(30, 30, 30, 30);
    
    // Title
    titleLabel_ = new QLabel("Menu");
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setFont(StyleManager::instance().getHeaderFont());
    titleLabel_->setStyleSheet("color: #333333; font-weight: bold;");
    panelLayout->addWidget(titleLabel_);
    
    // Button grid
    buttonLayout_ = new QGridLayout();
    buttonLayout_->setSpacing(15);
    
    // Create buttons
    newCaseButton_ = new QPushButton("New Case");
    openCaseButton_ = new QPushButton("Open Case");
    saveButton_ = new QPushButton("Save");
    saveAsButton_ = new QPushButton("Save As");
    increaseFontButton_ = new QPushButton("Font Size +");
    decreaseFontButton_ = new QPushButton("Font Size -");
    resetFontButton_ = new QPushButton("Reset Font");
    aboutButton_ = new QPushButton("About");
    exitButton_ = new QPushButton("Exit");
    
    // Set minimum height for all buttons
    QList<QPushButton*> buttons = {newCaseButton_, openCaseButton_, saveButton_, saveAsButton_,
                                   increaseFontButton_, decreaseFontButton_, resetFontButton_,
                                   aboutButton_, exitButton_};
    
    for (QPushButton* button : buttons) {
        button->setMinimumHeight(60);
        button->setMinimumWidth(160);
    }
    
    // Arrange buttons in grid (3 columns)
    buttonLayout_->addWidget(newCaseButton_, 0, 0);
    buttonLayout_->addWidget(openCaseButton_, 0, 1);
    buttonLayout_->addWidget(saveButton_, 0, 2);
    buttonLayout_->addWidget(saveAsButton_, 1, 0);
    buttonLayout_->addWidget(increaseFontButton_, 1, 1);
    buttonLayout_->addWidget(decreaseFontButton_, 1, 2);
    buttonLayout_->addWidget(resetFontButton_, 2, 0);
    buttonLayout_->addWidget(aboutButton_, 2, 1);
    buttonLayout_->addWidget(exitButton_, 2, 2);
    
    panelLayout->addLayout(buttonLayout_);
    panelLayout->addStretch();
    
    // Add close instruction
    QLabel* instructionLabel = new QLabel("Press ESC to close");
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setFont(StyleManager::instance().getBodyFont());
    instructionLabel->setStyleSheet("color: #666666; font-style: italic;");
    panelLayout->addWidget(instructionLabel);
    
    centerLayout->addWidget(menuPanel_);
    centerLayout->addStretch();
    
    mainLayout->addStretch();
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();
    
    // Connect signals
    connect(newCaseButton_, &QPushButton::clicked, this, &EscOverlayMenu::onNewCaseClicked);
    connect(openCaseButton_, &QPushButton::clicked, this, &EscOverlayMenu::onOpenCaseClicked);
    connect(saveButton_, &QPushButton::clicked, this, &EscOverlayMenu::onSaveClicked);
    connect(saveAsButton_, &QPushButton::clicked, this, &EscOverlayMenu::onSaveAsClicked);
    connect(increaseFontButton_, &QPushButton::clicked, this, &EscOverlayMenu::onIncreaseFontClicked);
    connect(decreaseFontButton_, &QPushButton::clicked, this, &EscOverlayMenu::onDecreaseFontClicked);
    connect(resetFontButton_, &QPushButton::clicked, this, &EscOverlayMenu::onResetFontClicked);
    connect(aboutButton_, &QPushButton::clicked, this, &EscOverlayMenu::onAboutClicked);
    connect(exitButton_, &QPushButton::clicked, this, &EscOverlayMenu::onExitClicked);
}

void EscOverlayMenu::setupButtonStyling()
{
    // Style buttons similar to case selection buttons
    
    // File operation buttons (blue theme)
    newCaseButton_->setStyleSheet(getButtonStyleSheet("#0066CC", true));
    openCaseButton_->setStyleSheet(getButtonStyleSheet("#0066CC", true));
    saveButton_->setStyleSheet(getButtonStyleSheet("#0066CC", true));
    saveAsButton_->setStyleSheet(getButtonStyleSheet("#0066CC", true));
    
    // Font buttons (green theme)
    increaseFontButton_->setStyleSheet(getButtonStyleSheet("#228B22"));
    decreaseFontButton_->setStyleSheet(getButtonStyleSheet("#228B22"));
    resetFontButton_->setStyleSheet(getButtonStyleSheet("#228B22"));
    
    // Utility buttons (gray theme)
    aboutButton_->setStyleSheet(getButtonStyleSheet("#666666"));
    exitButton_->setStyleSheet(getButtonStyleSheet("#DC143C")); // Red for exit
}

QString EscOverlayMenu::getButtonStyleSheet(const QString& color, bool /* isFileButton */) const
{
    QString textColor = (color == "#666666" || color == "#DC143C") ? "white" : "white";
    
    return QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: %2;"
        "   font-weight: bold;"
        "   font-size: 18px;"
        "   border: 2px solid %1;"
        "   border-radius: 8px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 0, 0, 0.1);"
        "   border-color: %1;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 0, 0, 0.2);"
        "}"
    ).arg(color).arg(textColor);
}

void EscOverlayMenu::showMenu()
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

void EscOverlayMenu::hideMenu()
{
    visible_ = false;
    hide();
    
    // Return focus to parent
    if (parentWidget()) {
        parentWidget()->setFocus();
    }
}

bool EscOverlayMenu::isMenuVisible() const
{
    return visible_;
}

void EscOverlayMenu::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    
    // Draw semi-transparent background
    painter.fillRect(rect(), QColor(0, 0, 0, 150));
    
    QWidget::paintEvent(event);
}

void EscOverlayMenu::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        hideMenu();
        event->accept();
        return;
    }
    
    QWidget::keyPressEvent(event);
}

void EscOverlayMenu::mousePressEvent(QMouseEvent* event)
{
    // Close menu if clicking outside the menu panel
    if (!menuPanel_->geometry().contains(event->pos())) {
        hideMenu();
        event->accept();
        return;
    }
    
    QWidget::mousePressEvent(event);
}

// Slot implementations
void EscOverlayMenu::onNewCaseClicked()
{
    hideMenu();
    emit newCaseRequested();
}

void EscOverlayMenu::onOpenCaseClicked()
{
    hideMenu();
    emit openCaseRequested();
}

void EscOverlayMenu::onSaveClicked()
{
    hideMenu();
    emit saveRequested();
}

void EscOverlayMenu::onSaveAsClicked()
{
    hideMenu();
    emit saveAsRequested();
}

void EscOverlayMenu::onIncreaseFontClicked()
{
    hideMenu();
    emit increaseFontRequested();
}

void EscOverlayMenu::onDecreaseFontClicked()
{
    hideMenu();
    emit decreaseFontRequested();
}

void EscOverlayMenu::onResetFontClicked()
{
    hideMenu();
    emit resetFontRequested();
}

void EscOverlayMenu::onAboutClicked()
{
    hideMenu();
    emit aboutRequested();
}

void EscOverlayMenu::onExitClicked()
{
    hideMenu();
    emit exitRequested();
}