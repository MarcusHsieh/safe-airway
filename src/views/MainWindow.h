#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QTimer>
#include <QLabel>
#include "models/Case.h"

class CaseSelectionView;
class TracheostomyFormView;
class NewTracheostomyFormView;
class DifficultAirwayFormView;
class LTRFormView;
class CaseManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onNewCaseRequested(CaseType caseType);
    void onExistingCaseSelected(const QString& filePath);
    void onCaseSaved(const QString& filePath);
    void onSaveRequested();
    void onBackToSelection();
    void onBackRequested();
    void onAutoSaveTimer();
    
    void onMenuFileNew();
    void onMenuFileOpen();
    void onMenuFileSave();
    void onMenuFileSaveAs();
    void onMenuFileExit();
    void onMenuViewIncreaseFontSize();
    void onMenuViewDecreaseFontSize();
    void onMenuViewResetFontSize();
    void onMenuHelpAbout();

private:
    QStackedWidget* stackedWidget_;
    
    CaseSelectionView* caseSelectionView_;
    TracheostomyFormView* tracheostomyFormView_;
    NewTracheostomyFormView* newTracheostomyFormView_;
    DifficultAirwayFormView* difficultAirwayFormView_;
    LTRFormView* ltrFormView_;
    
    QMenuBar* menuBar_;
    QStatusBar* statusBar_;
    QLabel* statusLabel_;
    QTimer* autoSaveTimer_;
    
    CaseManager* caseManager_;
    QString currentFilePath_;
    bool hasUnsavedChanges_;
    
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupConnections();
    void setupAutoSave();
    
    void showCaseSelectionView();
    void showFormView(CaseType caseType);
    void loadCase(const QString& filePath);
    void saveCase();
    void saveCurrentCase();
    bool saveAsCase();
    
    void updateWindowTitle();
    void updateStatusBar(const QString& message);
    void setUnsavedChanges(bool hasChanges);
    
    bool promptSaveChanges();
    void applyFontSize(int size);
    
    enum ViewIndex {
        CaseSelectionViewIndex = 0,
        TracheostomyFormViewIndex = 1,
        NewTracheostomyFormViewIndex = 2,
        DifficultAirwayFormViewIndex = 3,
        LTRFormViewIndex = 4
    };
};

#endif // MAINWINDOW_H