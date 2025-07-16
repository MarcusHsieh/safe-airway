#include "StyleManager.h"
#include <QApplication>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>

StyleManager* StyleManager::instance_ = nullptr;

StyleManager::StyleManager()
    : baseFontSize_(26) // Increased from 18 to 22 for even better readability on Surface Pro 8
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
        return QColor("#20B2AA");
    case CaseType::NewTracheostomy:
        return QColor("#FF1493");
    case CaseType::DifficultAirway:
        return QColor("#FFD700");
    case CaseType::LTR:
        return QColor("#4169E1");
    }
    return QColor("#20B2AA");
}

QFont StyleManager::getHeaderFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(baseFontSize_ + 4);
    font.setBold(true);
    return font;
}

QFont StyleManager::getBodyFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(baseFontSize_);
    return font;
}

QFont StyleManager::getTableFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(baseFontSize_ - 1);
    return font;
}

QFont StyleManager::getEmergencyFont() const
{
    QFont font = QApplication::font();
    font.setPointSize(baseFontSize_);
    font.setBold(true);
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
        "   font-size: %1px;"
        "   padding: 5px;"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 3px;"
        "}"
        "QPushButton {"
        "   font-size: %1px;"
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
    ).arg(baseFontSize_).arg(formColour.name()).arg(baseFontSize_ + 4);
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
        "   font-size: %1px;"
        "   font-weight: bold;"
        "   padding: 5px;"
        "   border: 1px solid #CCCCCC;"
        "}"
    ).arg(baseFontSize_ - 1);
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
        "   margin-top: 10px;"
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
        "   font-size: %1px;"
        "   font-weight: bold;"
        "}"
        "QRadioButton {"
        "   color: #DC143C;"
        "   font-size: %1px;"
        "   font-weight: bold;"
        "   padding: 3px;"
        "}"
        "QTextEdit {"
        "   background-color: white;"
        "   border: 1px solid #DC143C;"
        "   font-size: %1px;"
        "}"
    ).arg(baseFontSize_);
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