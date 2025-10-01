#include "StyleManager.h"
#include <QApplication>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>

// ============================================================================
// GLOBAL FONT SIZE CONFIGURATION
// ============================================================================
// Edit these values to change font sizes for specific UI elements

// Base font sizes
static const int BASE_FONT_SIZE = 32;                    // Base font for most elements

// Header and title fonts
static const int HEADER_FONT_SIZE = 36;                  // Main page headers
static const int FORM_HEADER_FONT_SIZE = 36;             // Form header labels
static const int PATIENT_NAME_FONT_SIZE = 40;            // Patient name display
static const int KEY_ELEMENT_FONT_SIZE = 36;             // Key form elements
static const int DECISION_BOX_FONT_SIZE = 38;            // Decision box labels

// Emergency panel fonts
static const int EMERGENCY_TITLE_FONT_SIZE = 32;         // "EMERGENCY SCENARIOS" title
static const int EMERGENCY_RADIO_FONT_SIZE = 26;         // Radio buttons (Can't Suction, etc.)
static const int EMERGENCY_INSTRUCTIONS_FONT_SIZE = 32;  // Emergency instructions text
static const int EMERGENCY_LABEL_FONT_SIZE = 32;        // "Emergency Instructions:" label

// Table fonts
static const int TABLE_FONT_SIZE = 31;                   // Table content
static const int TABLE_HEADER_FONT_SIZE = 31;            // Table headers

// Form element fonts
static const int BODY_FONT_SIZE = 32;                    // General body text
static const int FORM_LABEL_FONT_SIZE = 32;              // Form field labels
static const int FORM_INPUT_FONT_SIZE = 32;              // Form input fields
static const int BUTTON_FONT_SIZE = 32;                  // Button text
static const int GROUP_BOX_FONT_SIZE = 32;               // Group box titles

// Notification and status fonts
static const int NOTIFICATION_FONT_SIZE = 32;            // Notification messages
static const int STATUS_FONT_SIZE = 32;                  // Status indicators

// ============================================================================

StyleManager* StyleManager::instance_ = nullptr;

StyleManager::StyleManager()
    : baseFontSize_(BASE_FONT_SIZE)
{
}

StyleManager& StyleManager::instance()
{
    if (!instance_) {
        instance_ = new StyleManager();
    }
    return *instance_;
}

void StyleManager::applyFormStyle(QWidget* widget, CaseType caseType)
{
    if (!widget) return;
    
    widget->setStyleSheet(getFormStyleSheet(caseType));
}

void StyleManager::applyTableStyle(QWidget* tableWidget)
{
    if (!tableWidget) return;
    
    tableWidget->setStyleSheet(getTableStyleSheet());
    
    if (QTableWidget* table = qobject_cast<QTableWidget*>(tableWidget)) {
        table->setFont(getTableFont());
        table->horizontalHeader()->setFont(getTableFont());
        table->verticalHeader()->setFont(getTableFont());
        table->horizontalHeader()->setStretchLastSection(true);
        table->setAlternatingRowColors(true);
    }
}

void StyleManager::applyEmergencyStyle(QWidget* widget)
{
    if (!widget) return;
    
    widget->setStyleSheet(getEmergencyStyleSheet());
}

void StyleManager::applyFreezeOverlay(QWidget* widget, bool frozen)
{
    if (!widget) return;
    
    if (frozen) {
        widget->setStyleSheet(getFreezeOverlayStyleSheet());
    } else {
        widget->setStyleSheet("");
    }
}

ColourScheme StyleManager::getColourScheme(CaseType caseType) const
{
    ColourScheme scheme;
    scheme.formColour = getFormColour(caseType);
    return scheme;
}

QColor StyleManager::getFormColour(CaseType caseType) const
{
    switch (caseType) {
    case CaseType::Tracheostomy:
        return QColor("#00897B");  // Professional Teal
    case CaseType::NewTracheostomy:
        return QColor("#5E35B1");  // Professional Deep Purple
    case CaseType::DifficultAirway:
        return QColor("#FFB300");  // Professional Amber
    case CaseType::LTR:
        return QColor("#1976D2");  // Professional Blue
    }
    return QColor("#00897B");
}

QFont StyleManager::getHeaderFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(HEADER_FONT_SIZE);
    font.setBold(true);
    return font;
}

QFont StyleManager::getBodyFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(BODY_FONT_SIZE);
    return font;
}

QFont StyleManager::getTableFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(TABLE_FONT_SIZE);
    return font;
}

QFont StyleManager::getEmergencyFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(EMERGENCY_TITLE_FONT_SIZE);
    font.setBold(true);
    return font;
}

QFont StyleManager::getPatientNameFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(PATIENT_NAME_FONT_SIZE);
    font.setBold(true);
    return font;
}

QFont StyleManager::getKeyElementFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(KEY_ELEMENT_FONT_SIZE);
    font.setBold(true);
    return font;
}

QFont StyleManager::getDecisionBoxFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(DECISION_BOX_FONT_SIZE);
    font.setBold(true);
    return font;
}

// Emergency panel specific fonts
QFont StyleManager::getEmergencyTitleFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(EMERGENCY_TITLE_FONT_SIZE);
    font.setBold(true);
    return font;
}

QFont StyleManager::getEmergencyRadioFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(EMERGENCY_RADIO_FONT_SIZE);
    font.setBold(true);
    return font;
}

QFont StyleManager::getEmergencyInstructionsFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(EMERGENCY_INSTRUCTIONS_FONT_SIZE);
    return font;
}

QFont StyleManager::getEmergencyLabelFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(EMERGENCY_LABEL_FONT_SIZE);
    font.setBold(true);
    return font;
}

// Form element specific fonts
QFont StyleManager::getFormLabelFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(FORM_LABEL_FONT_SIZE);
    return font;
}

QFont StyleManager::getFormInputFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(FORM_INPUT_FONT_SIZE);
    return font;
}

QFont StyleManager::getButtonFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(BUTTON_FONT_SIZE);
    return font;
}

QFont StyleManager::getGroupBoxFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(GROUP_BOX_FONT_SIZE);
    font.setBold(false);  // Make group box titles non-bold to prevent text cutoff
    return font;
}

void StyleManager::setBaseFontSize(int size)
{
    baseFontSize_ = size;
    updateFontSizes();
}

int StyleManager::getBaseFontSize() const
{
    return baseFontSize_;
}

int StyleManager::getNotificationFontSize() const
{
    return NOTIFICATION_FONT_SIZE;
}

void StyleManager::updateFontSizes()
{
    QFont appFont = QApplication::font();
    appFont.setPointSize(baseFontSize_);
    QApplication::setFont(appFont);
}

QString StyleManager::getFormStyleSheet(CaseType caseType) const
{
    QColor formColour = getFormColour(caseType);
    
    return QString(
        "QWidget {"
        "   background-color: white;"
        "   font-size: %1px;"
        "}"
        "QLabel#headerLabel {"
        "   background-color: %2;"
        "   color: white;"
        "   font-size: %3px;"
        "   font-weight: bold;"
        "   padding: 10px;"
        "   border-radius: 5px;"
        "}"
        "QLineEdit, QTextEdit, QComboBox, QSpinBox {"
        "   font-size: %4px;"
        "   padding: 5px;"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 3px;"
        "}"
        "QPushButton {"
        "   font-size: %5px;"
        "   padding: 8px 16px;"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 3px;"
        "   background-color: #F5F5F5;"
        "}"
        "QPushButton:hover {"
        "   background-color: #E6E6E6;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #D0D0D0;"
        "}"
        "QGroupBox {"
        "   font-size: %6px;"
        "   font-weight: normal;"
        "   color: #333333;"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 5px;"
        "   margin-top: 25px;"
        "   padding-top: 15px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 8px 0 8px;"
        "   background-color: white;"
        "}"
    ).arg(BODY_FONT_SIZE).arg(formColour.name()).arg(FORM_HEADER_FONT_SIZE).arg(FORM_INPUT_FONT_SIZE).arg(BUTTON_FONT_SIZE).arg(GROUP_BOX_FONT_SIZE);
}

QString StyleManager::getTableStyleSheet() const
{
    return QString(
        "QTableWidget {"
        "   gridline-color: #CCCCCC;"
        "   font-size: %1px;"
        "   selection-background-color: #B0C4DE;"
        "   alternate-background-color: #F8F8F8;"
        "}"
        "QTableWidget::item {"
        "   padding: 5px;"
        "   border-bottom: 1px solid #CCCCCC;"
        "}"
        "QHeaderView::section {"
        "   background-color: #E6E6E6;"
        "   font-size: %2px;"
        "   font-weight: normal;"
        "   padding: 5px;"
        "   border: 1px solid #CCCCCC;"
        "}"
    ).arg(TABLE_FONT_SIZE).arg(TABLE_HEADER_FONT_SIZE);
}

QString StyleManager::getEmergencyStyleSheet() const
{
    return QString(
        "QWidget {"
        "   background-color: #FFE4E1;"
        "   border: 2px solid #DC143C;"
        "   border-radius: 5px;"
        "}"
        "QGroupBox {"
        "   font-size: %1px;"
        "   font-weight: bold;"
        "   color: #DC143C;"
        "   border: 2px solid #DC143C;"
        "   border-radius: 5px;"
        "   margin-top: 25px;"
        "   padding-top: 15px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 8px 0 8px;"
        "   background-color: #FFE4E1;"
        "}"
        "QLabel {"
        "   color: #DC143C;"
        "   font-size: %2px;"
        "   font-weight: bold;"
        "}"
        "QPushButton {"
        "   color: #DC143C;"
        "   font-size: %3px;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "   border: 2px solid #DC143C;"
        "   border-radius: 5px;"
        "   background-color: #FFE4E1;"
        "   text-align: center;"
        "}"
        "QPushButton:checked {"
        "   background-color: #DC143C;"
        "   color: white;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFC0CB;"
        "}"
        "QPushButton:checked:hover {"
        "   background-color: #B22222;"
        "}"
        "QTextEdit {"
        "   background-color: white;"
        "   border: 1px solid #DC143C;"
        "   font-size: %4px;"
        "}"
    ).arg(GROUP_BOX_FONT_SIZE).arg(EMERGENCY_LABEL_FONT_SIZE).arg(EMERGENCY_RADIO_FONT_SIZE).arg(EMERGENCY_INSTRUCTIONS_FONT_SIZE);
}

QString StyleManager::getFreezeOverlayStyleSheet() const
{
    return QString(
        "QWidget {"
        "   background-color: rgba(120, 120, 120, 60);"
        "}"
        "QLineEdit, QTextEdit, QComboBox, QSpinBox, QTableWidget {"
        "   background-color: rgba(140, 140, 140, 40);"
        "   color: #505050;"
        "}"
        "QGroupBox {"
        "   background-color: rgba(120, 120, 120, 40);"
        "   border: 1px solid #808080;"
        "}"
        "QLabel {"
        "   color: #505050;"
        "}"
    );
}