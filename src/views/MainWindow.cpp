#include "MainWindow.h"
#include "CaseSelectionView.h"
#include "TracheostomyFormView.h"
#include "NewTracheostomyFormView.h"
#include "DifficultAirwayFormView.h"
#include "LTRFormView.h"
#include "widgets/BaseFormWidget.h"
#include "core/Application.h"
#include "core/CaseManager.h"
#include "utils/StyleManager.h"
#include "utils/ConfigManager.h"
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , stackedWidget_(nullptr)
    , caseSelectionView_(nullptr)
    , tracheostomyFormView_(nullptr)
    , newTracheostomyFormView_(nullptr)
    , difficultAirwayFormView_(nullptr)
    , ltrFormView_(nullptr)
    , menuBar_(nullptr)
    , statusBar_(nullptr)
    , statusLabel_(nullptr)
    , autoSaveTimer_(nullptr)
    , caseManager_(nullptr)
    , hasUnsavedChanges_(false)
{
    caseManager_ = Application::instance().getCaseManager();
    
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();
    setupAutoSave();
    
    showCaseSelectionView();
    updateWindowTitle();
    updateStatusBar("Ready");
    
    QSize windowSize;
    QPoint windowPosition;
    ConfigManager::instance().loadWindowGeometry("MainWindow", windowSize, windowPosition);
    resize(windowSize);
    move(windowPosition);
    
    // Start in fullscreen mode by default for Surface Pro 8
    showFullScreen();
}

MainWindow::~MainWindow()
{
    QSize windowSize = size();
    QPoint windowPosition = pos();
    ConfigManager::instance().saveWindowGeometry("MainWindow", windowSize, windowPosition);
}

void MainWindow::setupUI()
{
    stackedWidget_ = new QStackedWidget();
    setCentralWidget(stackedWidget_);
    
    caseSelectionView_ = new CaseSelectionView();
    tracheostomyFormView_ = new TracheostomyFormView();
    newTracheostomyFormView_ = new NewTracheostomyFormView();
    difficultAirwayFormView_ = new DifficultAirwayFormView();
    ltrFormView_ = new LTRFormView();
    
    // Complete the setup of form-specific fields after derived class construction
    tracheostomyFormView_->finishSetup();
    newTracheostomyFormView_->finishSetup();
    difficultAirwayFormView_->finishSetup();
    ltrFormView_->finishSetup();
    
    stackedWidget_->addWidget(caseSelectionView_);
    stackedWidget_->addWidget(tracheostomyFormView_);
    stackedWidget_->addWidget(newTracheostomyFormView_);
    stackedWidget_->addWidget(difficultAirwayFormView_);
    stackedWidget_->addWidget(ltrFormView_);
}

void MainWindow::setupMenuBar()
{
    menuBar_ = menuBar();
    
    QMenu* fileMenu = menuBar_->addMenu("&File");
    
    QAction* newAction = new QAction("&New Case", this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::onMenuFileNew);
    fileMenu->addAction(newAction);
    
    QAction* openAction = new QAction("&Open Case", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onMenuFileOpen);
    fileMenu->addAction(openAction);
    
    fileMenu->addSeparator();
    
    QAction* saveAction = new QAction("&Save", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onMenuFileSave);
    fileMenu->addAction(saveAction);
    
    QAction* saveAsAction = new QAction("Save &As...", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onMenuFileSaveAs);
    fileMenu->addAction(saveAsAction);
    
    fileMenu->addSeparator();
    
    QAction* exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onMenuFileExit);
    fileMenu->addAction(exitAction);
    
    QMenu* viewMenu = menuBar_->addMenu("&View");
    
    QAction* increaseFontAction = new QAction("Increase Font Size", this);
    increaseFontAction->setShortcut(QKeySequence::ZoomIn);
    connect(increaseFontAction, &QAction::triggered, this, &MainWindow::onMenuViewIncreaseFontSize);
    viewMenu->addAction(increaseFontAction);
    
    QAction* decreaseFontAction = new QAction("Decrease Font Size", this);
    decreaseFontAction->setShortcut(QKeySequence::ZoomOut);
    connect(decreaseFontAction, &QAction::triggered, this, &MainWindow::onMenuViewDecreaseFontSize);
    viewMenu->addAction(decreaseFontAction);
    
    QAction* resetFontAction = new QAction("Reset Font Size", this);
    connect(resetFontAction, &QAction::triggered, this, &MainWindow::onMenuViewResetFontSize);
    viewMenu->addAction(resetFontAction);
    
    QMenu* helpMenu = menuBar_->addMenu("&Help");
    
    QAction* aboutAction = new QAction("&About Safe Airway", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onMenuHelpAbout);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupStatusBar()
{
    statusBar_ = statusBar();
    statusLabel_ = new QLabel();
    statusBar_->addWidget(statusLabel_);
}

void MainWindow::setupConnections()
{
    connect(caseSelectionView_, &CaseSelectionView::newCaseRequested, this, &MainWindow::onNewCaseRequested);
    connect(caseSelectionView_, &CaseSelectionView::existingCaseSelected, this, &MainWindow::onExistingCaseSelected);
    
    connect(tracheostomyFormView_, &TracheostomyFormView::saveRequested, this, &MainWindow::onSaveRequested);
    connect(newTracheostomyFormView_, &NewTracheostomyFormView::saveRequested, this, &MainWindow::onSaveRequested);
    connect(difficultAirwayFormView_, &DifficultAirwayFormView::saveRequested, this, &MainWindow::onSaveRequested);
    connect(ltrFormView_, &LTRFormView::saveRequested, this, &MainWindow::onSaveRequested);
    
    // Connect back button signals
    connect(tracheostomyFormView_, &TracheostomyFormView::backRequested, this, &MainWindow::onBackRequested);
    connect(newTracheostomyFormView_, &NewTracheostomyFormView::backRequested, this, &MainWindow::onBackRequested);
    connect(difficultAirwayFormView_, &DifficultAirwayFormView::backRequested, this, &MainWindow::onBackRequested);
    connect(ltrFormView_, &LTRFormView::backRequested, this, &MainWindow::onBackRequested);
    
    connect(tracheostomyFormView_, &TracheostomyFormView::formChanged, this, [this]() { setUnsavedChanges(true); });
    connect(newTracheostomyFormView_, &NewTracheostomyFormView::formChanged, this, [this]() { setUnsavedChanges(true); });
    connect(difficultAirwayFormView_, &DifficultAirwayFormView::formChanged, this, [this]() { setUnsavedChanges(true); });
    connect(ltrFormView_, &LTRFormView::formChanged, this, [this]() { setUnsavedChanges(true); });
    
    connect(caseManager_, &CaseManager::caseSaved, this, &MainWindow::onCaseSaved);
    connect(caseManager_, &CaseManager::error, this, [this](const QString& message) {
        QMessageBox::critical(this, "Error", message);
        updateStatusBar("Error: " + message);
    });
}

void MainWindow::setupAutoSave()
{
    autoSaveTimer_ = new QTimer(this);
    connect(autoSaveTimer_, &QTimer::timeout, this, &MainWindow::onAutoSaveTimer);
    
    int interval = ConfigManager::instance().getAutoSaveInterval();
    if (interval > 0) {
        autoSaveTimer_->start(interval * 60 * 1000);
    }
}

void MainWindow::showCaseSelectionView()
{
    stackedWidget_->setCurrentIndex(CaseSelectionViewIndex);
    updateWindowTitle();
}

void MainWindow::showFormView(CaseType caseType)
{
    switch (caseType) {
    case CaseType::Tracheostomy:
        stackedWidget_->setCurrentIndex(TracheostomyFormViewIndex);
        break;
    case CaseType::NewTracheostomy:
        stackedWidget_->setCurrentIndex(NewTracheostomyFormViewIndex);
        break;
    case CaseType::DifficultAirway:
        stackedWidget_->setCurrentIndex(DifficultAirwayFormViewIndex);
        break;
    case CaseType::LTR:
        stackedWidget_->setCurrentIndex(LTRFormViewIndex);
        break;
    }
    updateWindowTitle();
}

void MainWindow::onNewCaseRequested(CaseType caseType)
{
    QWidget* currentWidget = stackedWidget_->currentWidget();
    BaseFormWidget* currentFormWidget = nullptr;
    
    // Check current form's save status if we're on a form
    if (currentWidget == tracheostomyFormView_) {
        currentFormWidget = tracheostomyFormView_;
    } else if (currentWidget == newTracheostomyFormView_) {
        currentFormWidget = newTracheostomyFormView_;
    } else if (currentWidget == difficultAirwayFormView_) {
        currentFormWidget = difficultAirwayFormView_;
    } else if (currentWidget == ltrFormView_) {
        currentFormWidget = ltrFormView_;
    }
    
    // Only prompt if there are unsaved changes and the form wasn't just saved
    if (currentFormWidget && !currentFormWidget->wasJustSaved() && hasUnsavedChanges_) {
        if (!promptSaveChanges()) {
            return;
        }
    }
    
    Case newCase(QString(), caseType);
    currentFilePath_.clear();
    setUnsavedChanges(false);
    
    switch (caseType) {
    case CaseType::Tracheostomy:
        tracheostomyFormView_->setCase(newCase);
        break;
    case CaseType::NewTracheostomy:
        newTracheostomyFormView_->setCase(newCase);
        break;
    case CaseType::DifficultAirway:
        difficultAirwayFormView_->setCase(newCase);
        break;
    case CaseType::LTR:
        ltrFormView_->setCase(newCase);
        break;
    }
    
    showFormView(caseType);
    updateStatusBar("New case created");
}

void MainWindow::onExistingCaseSelected(const QString& filePath)
{
    if (hasUnsavedChanges_ && !promptSaveChanges()) {
        return;
    }
    
    loadCase(filePath);
}

void MainWindow::onCaseSaved(const QString& filePath)
{
    currentFilePath_ = filePath;
    setUnsavedChanges(false);
    updateStatusBar("Case saved successfully");
    updateWindowTitle();
}

void MainWindow::onSaveRequested()
{
    saveCurrentCase();
}

void MainWindow::onBackToSelection()
{
    if (hasUnsavedChanges_ && !promptSaveChanges()) {
        return;
    }
    
    showCaseSelectionView();
}

void MainWindow::onBackRequested()
{
    QWidget* currentWidget = stackedWidget_->currentWidget();
    BaseFormWidget* formWidget = nullptr;
    
    if (currentWidget == tracheostomyFormView_) {
        formWidget = tracheostomyFormView_;
    } else if (currentWidget == newTracheostomyFormView_) {
        formWidget = newTracheostomyFormView_;
    } else if (currentWidget == difficultAirwayFormView_) {
        formWidget = difficultAirwayFormView_;
    } else if (currentWidget == ltrFormView_) {
        formWidget = ltrFormView_;
    }
    
    if (formWidget) {
        // If just saved, or if there are no unsaved changes, go back directly
        if (formWidget->wasJustSaved() || !hasUnsavedChanges_) {
            showCaseSelectionView();
        } else {
            // There are unsaved changes, prompt user
            if (promptSaveChanges()) {
                showCaseSelectionView();
            }
        }
    } else {
        // Fallback to regular behavior
        showCaseSelectionView();
    }
}

void MainWindow::onAutoSaveTimer()
{
    if (hasUnsavedChanges_ && !currentFilePath_.isEmpty()) {
        saveCurrentCase();
        updateStatusBar("Auto-saved");
    }
}

void MainWindow::loadCase(const QString& filePath)
{
    Case case_;
    if (!caseManager_->loadCase(filePath, case_)) {
        QMessageBox::critical(this, "Error", "Failed to load case from: " + filePath);
        return;
    }
    
    currentFilePath_ = filePath;
    setUnsavedChanges(false);
    
    switch (case_.getCaseType()) {
    case CaseType::Tracheostomy:
        tracheostomyFormView_->setCase(case_);
        break;
    case CaseType::NewTracheostomy:
        newTracheostomyFormView_->setCase(case_);
        break;
    case CaseType::DifficultAirway:
        difficultAirwayFormView_->setCase(case_);
        break;
    case CaseType::LTR:
        ltrFormView_->setCase(case_);
        break;
    }
    
    showFormView(case_.getCaseType());
    updateStatusBar("Case loaded successfully");
}

void MainWindow::saveCurrentCase()
{
    QWidget* currentWidget = stackedWidget_->currentWidget();
    if (!currentWidget) return;
    
    Case case_;
    
    if (currentWidget == tracheostomyFormView_) {
        case_ = tracheostomyFormView_->getCase();
    } else if (currentWidget == newTracheostomyFormView_) {
        case_ = newTracheostomyFormView_->getCase();
    } else if (currentWidget == difficultAirwayFormView_) {
        case_ = difficultAirwayFormView_->getCase();
    } else if (currentWidget == ltrFormView_) {
        case_ = ltrFormView_->getCase();
    } else {
        return;
    }
    
    QString filePath = caseManager_->saveCase(case_);
    if (!filePath.isEmpty()) {
        currentFilePath_ = filePath;
        setUnsavedChanges(false);
        updateWindowTitle();
    }
}

bool MainWindow::saveAsCase()
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        "Save Case As", 
        caseManager_->getBasePath(),
        "JSON files (*.json)");
    
    if (fileName.isEmpty()) {
        return false;
    }
    
    QWidget* currentWidget = stackedWidget_->currentWidget();
    if (!currentWidget) return false;
    
    Case case_;
    
    if (currentWidget == tracheostomyFormView_) {
        case_ = tracheostomyFormView_->getCase();
    } else if (currentWidget == newTracheostomyFormView_) {
        case_ = newTracheostomyFormView_->getCase();
    } else if (currentWidget == difficultAirwayFormView_) {
        case_ = difficultAirwayFormView_->getCase();
    } else if (currentWidget == ltrFormView_) {
        case_ = ltrFormView_->getCase();
    } else {
        return false;
    }
    
    if (caseManager_->exportCase(case_, fileName)) {
        currentFilePath_ = fileName;
        setUnsavedChanges(false);
        updateWindowTitle();
        updateStatusBar("Case saved as: " + fileName);
        return true;
    }
    
    return false;
}

void MainWindow::updateWindowTitle()
{
    QString title = "Safe Airway";
    
    if (!currentFilePath_.isEmpty()) {
        QFileInfo fileInfo(currentFilePath_);
        title += " - " + fileInfo.baseName();
    }
    
    if (hasUnsavedChanges_) {
        title += " *";
    }
    
    setWindowTitle(title);
}

void MainWindow::updateStatusBar(const QString& message)
{
    statusLabel_->setText(message);
}

void MainWindow::setUnsavedChanges(bool hasChanges)
{
    hasUnsavedChanges_ = hasChanges;
    updateWindowTitle();
}

bool MainWindow::promptSaveChanges()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Unsaved Changes", 
        "Do you want to save your changes before continuing?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    
    if (reply == QMessageBox::Save) {
        saveCurrentCase();
        return true;
    } else if (reply == QMessageBox::Discard) {
        return true;
    }
    
    return false;
}

void MainWindow::applyFontSize(int size)
{
    StyleManager::instance().setBaseFontSize(size);
    ConfigManager::instance().saveFontSize(size);
    updateStatusBar(QString("Font size changed to %1").arg(size));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (hasUnsavedChanges_ && !promptSaveChanges()) {
        event->ignore();
        return;
    }
    
    event->accept();
}

void MainWindow::onMenuFileNew()
{
    showCaseSelectionView();
}

void MainWindow::onMenuFileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        "Open Case", 
        caseManager_->getBasePath(),
        "JSON files (*.json)");
    
    if (!fileName.isEmpty()) {
        loadCase(fileName);
    }
}

void MainWindow::onMenuFileSave()
{
    saveCurrentCase();
}

void MainWindow::onMenuFileSaveAs()
{
    saveAsCase();
}

void MainWindow::onMenuFileExit()
{
    close();
}

void MainWindow::onMenuViewIncreaseFontSize()
{
    int currentSize = StyleManager::instance().getBaseFontSize();
    applyFontSize(currentSize + 2);
}

void MainWindow::onMenuViewDecreaseFontSize()
{
    int currentSize = StyleManager::instance().getBaseFontSize();
    if (currentSize > 8) {
        applyFontSize(currentSize - 2);
    }
}

void MainWindow::onMenuViewResetFontSize()
{
    applyFontSize(26);
}

void MainWindow::onMenuHelpAbout()
{
    QMessageBox::about(this, "About Safe Airway", 
        "Safe Airway v1.0.0\n\n"
        "A digital form application for airway management at Nemours Children's Health.\n\n"
        "© 2024 Nemours Children's Health");
}