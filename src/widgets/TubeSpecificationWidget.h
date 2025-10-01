#ifndef TUBESPECIFICATIONWIDGET_H
#define TUBESPECIFICATIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include "models/Case.h"

class TubeSpecificationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TubeSpecificationWidget(QWidget* parent = nullptr);
    
    void setSpecification(const SpecificationTableRow& spec);
    SpecificationTableRow getSpecification() const;
    
    void clear();
    void setReadOnly(bool readOnly);
    
    bool isValid() const;
    QStringList getValidationErrors() const;

signals:
    void specificationChanged();
    void manufacturerChanged(const QString& manufacturer);
    void sizeChanged(const QString& size);

private slots:
    void onManufacturerChanged();
    void onSizeChanged();
    void onFieldChanged();
    void updateCalculatedFields();

private:
    void setupUI();
    void setupStyling();
    void connectSignals();
    void populateManufacturerCombo();
    void populateSizeComboForManufacturer(const QString& manufacturer);
    
    // Input fields
    QComboBox* manufacturerCombo_;
    QComboBox* sizeCombo_;
    QLineEdit* typeEdit_;
    QLineEdit* cuffEdit_;
    QLineEdit* lengthEdit_;
    
    // Calculated/Display fields
    QLabel* innerDiameterLabel_;
    QLabel* outerDiameterLabel_;
    QLabel* lengthLabel_;
    QLabel* suctionCatheterLabel_;
    QLineEdit* reorderEdit_;
    
    // Layout components
    QGroupBox* inputGroup_;
    QGroupBox* calculatedGroup_;
    QFrame* separator_;
    
    // State
    bool readOnly_;
    bool updating_;
};

#endif // TUBESPECIFICATIONWIDGET_H