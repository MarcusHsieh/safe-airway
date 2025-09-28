#ifndef EMERGENCYPANEL_H
#define EMERGENCYPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QTextOption>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "models/EmergencyScenario.h"

class EmergencyPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EmergencyPanel(QWidget* parent = nullptr);
    
    void setSelectedScenario(const QString& scenarioName);
    QString getSelectedScenario() const;
    
    void setSuctionSize(int size);
    int getSuctionSize() const { return suctionSize_; }
    
    void setFrozen(bool frozen);
    bool isFrozen() const { return frozen_; }
    
    void clear();

signals:
    void scenarioSelected(const QString& scenarioName);
    void instructionsChanged(const QString& instructions);

private slots:
    void onScenarioButtonClicked();
    void updateInstructions();

private:
    QButtonGroup* scenarioGroup_;
    QPushButton* cantSuctionButton_;
    QPushButton* cantVentilateButton_;
    QPushButton* o2SatDropButton_;
    QPushButton* decannulationButton_;
    QPushButton* hemoptysisButton_;
    
    QTextEdit* instructionsEdit_;
    QLabel* titleLabel_;
    QGroupBox* scenarioGroupBox_;
    
    int suctionSize_;
    bool frozen_;
    
    void setupUI();
    void connectSignals();
    void updateStyles();
    QString getInstructionsWithETTSize(const QString& instructions) const;
};

#endif // EMERGENCYPANEL_H