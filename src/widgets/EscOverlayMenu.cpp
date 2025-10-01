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
    
    // Menu panel with modern card styling
    menuPanel_ = new QWidget();
    menuPanel_->setFixedSize(700, 550);
    menuPanel_->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 12px;"
        "}"
    );

    QVBoxLayout* panelLayout = new QVBoxLayout(menuPanel_);
    panelLayout->setSpacing(25);
    panelLayout->setContentsMargins(40, 35, 40, 35);

    // Title
    titleLabel_ = new QLabel("Menu");
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setFont(StyleManager::instance().getHeaderFont());
    titleLabel_->setStyleSheet("color: #2C3E50; font-weight: bold;");
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
    
    // Set minimum height and cursor for all buttons
    QList<QPushButton*> buttons = {newCaseButton_, openCaseButton_, saveButton_, saveAsButton_,
                                   increaseFontButton_, decreaseFontButton_, resetFontButton_,
                                   aboutButton_, exitButton_};

    for (QPushButton* button : buttons) {
        button->setMinimumHeight(65);
        button->setMinimumWidth(180);
        button->setCursor(Qt::PointingHandCursor);
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
    // Modern button styling matching the app theme

    // File operation buttons (blue theme)
    newCaseButton_->setStyleSheet(getButtonStyleSheet("#1976D2", "#1565C0"));
    openCaseButton_->setStyleSheet(getButtonStyleSheet("#1976D2", "#1565C0"));
    saveButton_->setStyleSheet(getButtonStyleSheet("#1976D2", "#1565C0"));
    saveAsButton_->setStyleSheet(getButtonStyleSheet("#1976D2", "#1565C0"));

    // Font buttons (teal theme)
    increaseFontButton_->setStyleSheet(getButtonStyleSheet("#00897B", "#00695C"));
    decreaseFontButton_->setStyleSheet(getButtonStyleSheet("#00897B", "#00695C"));
    resetFontButton_->setStyleSheet(getButtonStyleSheet("#00897B", "#00695C"));

    // Utility buttons
    aboutButton_->setStyleSheet(getButtonStyleSheet("#607D8B", "#546E7A"));
    exitButton_->setStyleSheet(getButtonStyleSheet("#DC143C", "#B22222")); // Red for exit
}

QString EscOverlayMenu::getButtonStyleSheet(const QString& color, bool isFileButton) const
{
    Q_UNUSED(isFileButton);

    // Calculate hover color (20% darker)
    QString hoverColor = color;
    if (color.contains("#")) {
        // Use the second color parameter as hover color
        hoverColor = color;
    }

    return QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 24px;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: %2;"
        "   padding-top: 14px;"
        "   padding-bottom: 10px;"
        "}"
    ).arg(color);
}

QString EscOverlayMenu::getButtonStyleSheet(const QString& color, const QString& hoverColor) const
{
    return QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 24px;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: %2;"
        "   padding-top: 14px;"
        "   padding-bottom: 10px;"
        "}"
    ).arg(color, hoverColor);
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