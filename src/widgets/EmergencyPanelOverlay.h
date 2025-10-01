#ifndef EMERGENCYPANELOVERLAY_H
#define EMERGENCYPANELOVERLAY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QButtonGroup>
#include <QGroupBox>

class EmergencyPanelOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit EmergencyPanelOverlay(QWidget* parent = nullptr);
    
    void showOverlay();
    void hideOverlay();
    bool isOverlayVisible() const;
    
    QString getSelectedScenario() const;
    void setSelectedScenario(const QString& scenarioName);
    void setSuctionSize(int size);

signals:
    void scenarioSelected(const QString& scenario);
    void instructionsChanged(const QString& instructions);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onScenarioButtonClicked();

private:
    void setupUI();
    void setupButtonStyling();
    void connectSignals();
    void updateInstructions();
    QString getInstructionsWithETTSize(const QString& instructions) const;
    QString getButtonStyleSheet(const QString& color) const;

    // Main components
    QWidget* overlayPanel_;
    QVBoxLayout* panelLayout_;
    QLabel* titleLabel_;
    
    // Emergency scenarios
    QGroupBox* scenarioGroupBox_;
    QButtonGroup* scenarioGroup_;
    QPushButton* cantSuctionButton_;
    QPushButton* cantVentilateButton_;
    QPushButton* o2SatDropButton_;
    QPushButton* decannulationButton_;
    QPushButton* hemoptysisButton_;
    
    // Instructions
    QTextEdit* instructionsEdit_;
    
    // Close button
    QPushButton* closeButton_;
    
    // State
    int suctionSize_;
    bool visible_;
};

#endif // EMERGENCYPANELOVERLAY_H