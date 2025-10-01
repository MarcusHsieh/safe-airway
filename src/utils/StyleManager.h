#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QColor>
#include <QWidget>
#include <QFont>
#include <QString>
#include "models/Case.h"

struct ColourScheme {
    QColor formColour;
    QColor emergencyRed = QColor("#DC143C");
    QColor freezeOverlay = QColor(120, 120, 120, 80);
    QColor headerBackground = QColor("#F5F5F5");
    QColor tableHeaderBackground = QColor("#E6E6E6");
    QColor tableBorder = QColor("#CCCCCC");
};

class StyleManager
{
public:
    static StyleManager& instance();
    
    void applyFormStyle(QWidget* widget, CaseType caseType);
    void applyTableStyle(QWidget* tableWidget);
    void applyEmergencyStyle(QWidget* widget);
    void applyFreezeOverlay(QWidget* widget, bool frozen);
    
    ColourScheme getColourScheme(CaseType caseType) const;
    QColor getFormColour(CaseType caseType) const;
    
    QFont getHeaderFont() const;
    QFont getBodyFont() const;
    QFont getTableFont() const;
    QFont getEmergencyFont() const;
    QFont getPatientNameFont() const;
    QFont getKeyElementFont() const;
    QFont getDecisionBoxFont() const;
    
    // Emergency panel specific fonts
    QFont getEmergencyTitleFont() const;
    QFont getEmergencyRadioFont() const;
    QFont getEmergencyInstructionsFont() const;
    QFont getEmergencyLabelFont() const;
    
    // Form element specific fonts
    QFont getFormLabelFont() const;
    QFont getFormInputFont() const;
    QFont getButtonFont() const;
    QFont getGroupBoxFont() const;
    
    void setBaseFontSize(int size);
    int getBaseFontSize() const;
    int getNotificationFontSize() const;

    QString getFormStyleSheet(CaseType caseType) const;
    QString getTableStyleSheet() const;
    QString getEmergencyStyleSheet() const;
    QString getFreezeOverlayStyleSheet() const;
    
private:
    StyleManager();
    
    static StyleManager* instance_;
    int baseFontSize_;
    
    void updateFontSizes();
};

#endif // STYLEMANAGER_H