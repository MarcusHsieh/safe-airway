#ifndef CASESELECTIONVIEW_H
#define CASESELECTIONVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QGroupBox>
#include <QSplitter>
#include "models/Case.h"

class CaseSelectionView : public QWidget
{
    Q_OBJECT

public:
    explicit CaseSelectionView(QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* event) override;

signals:
    void caseTypeSelected(CaseType caseType);
    void existingCaseSelected(const QString& filePath);
    void newCaseRequested(CaseType caseType);

private slots:
    void onNewCaseClicked();
    void onLoadCaseClicked();
    void onCaseTypeButtonClicked();
    void onRecentCaseDoubleClicked();
    void refreshRecentCases();

private:
    QVBoxLayout* mainLayout_;
    QLabel* titleLabel_;
    QLabel* logoLabel_;
    
    QGroupBox* newCaseGroup_;
    QPushButton* tracheostomyButton_;
    QPushButton* newTracheostomyButton_;
    QPushButton* difficultAirwayButton_;
    QPushButton* ltrButton_;
    
    QGroupBox* existingCaseGroup_;
    QListWidget* recentCasesList_;
    QPushButton* loadCaseButton_;
    QPushButton* refreshButton_;
    
    CaseType selectedCaseType_;
    
    void setupUI();
    void setupNewCaseSection();
    void setupExistingCaseSection();
    void updateStyles();
    void loadRecentCases();
    QString formatCaseDisplayName(const QString& filePath) const;
};

#endif // CASESELECTIONVIEW_H