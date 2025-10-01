#include "TubeSpecificationWidget.h"
#include "utils/StyleManager.h"
#include "utils/SATrachTube.h"
#include <QGuiApplication>
#include <QScreen>

TubeSpecificationWidget::TubeSpecificationWidget(QWidget* parent)
    : QWidget(parent)
    , manufacturerCombo_(nullptr)
    , sizeCombo_(nullptr)
    , typeEdit_(nullptr)
    , cuffEdit_(nullptr)
    , innerDiameterLabel_(nullptr)
    , outerDiameterLabel_(nullptr)
    , lengthLabel_(nullptr)
    , suctionCatheterLabel_(nullptr)
    , reorderEdit_(nullptr)
    , inputGroup_(nullptr)
    , calculatedGroup_(nullptr)
    , separator_(nullptr)
    , readOnly_(false)
    , updating_(false)
{
    setupUI();
    setupStyling();
    connectSignals();
    populateManufacturerCombo();
}

void TubeSpecificationWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    
    // Input section - horizontal label-field pairs stacked vertically
    inputGroup_ = new QGroupBox("Tube Selection");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup_);
    inputLayout->setSpacing(10);
    
    // Manufacturer selection
    QHBoxLayout* manufacturerLayout = new QHBoxLayout();
    QLabel* manufacturerLabel = new QLabel("Manufacturer:");
    manufacturerLabel->setMinimumWidth(120);
    manufacturerCombo_ = new QComboBox();
    manufacturerCombo_->setMinimumHeight(40);
    manufacturerLayout->addWidget(manufacturerLabel);
    manufacturerLayout->addWidget(manufacturerCombo_);
    inputLayout->addLayout(manufacturerLayout);
    
    // Size selection
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    QLabel* sizeLabel = new QLabel("Size (mm):");
    sizeLabel->setMinimumWidth(120);
    sizeCombo_ = new QComboBox();
    sizeCombo_->setMinimumHeight(40);
    sizeCombo_->setEditable(true);
    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(sizeCombo_);
    inputLayout->addLayout(sizeLayout);
    
    // Length field
    QHBoxLayout* lengthLayout = new QHBoxLayout();
    QLabel* lengthInputLabel = new QLabel("Length (mm):");
    lengthInputLabel->setMinimumWidth(120);
    lengthEdit_ = new QLineEdit();
    lengthEdit_->setMinimumHeight(40);
    lengthEdit_->setPlaceholderText("e.g., 80, 90");
    lengthLayout->addWidget(lengthInputLabel);
    lengthLayout->addWidget(lengthEdit_);
    inputLayout->addLayout(lengthLayout);
    
    // Type field
    QHBoxLayout* typeLayout = new QHBoxLayout();
    QLabel* typeLabel = new QLabel("Type:");
    typeLabel->setMinimumWidth(120);
    typeEdit_ = new QLineEdit();
    typeEdit_->setMinimumHeight(40);
    typeEdit_->setPlaceholderText("e.g., Pediatric, Neonatal");
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeEdit_);
    inputLayout->addLayout(typeLayout);
    
    // Cuff field
    QHBoxLayout* cuffLayout = new QHBoxLayout();
    QLabel* cuffLabel = new QLabel("Cuff:");
    cuffLabel->setMinimumWidth(120);
    cuffEdit_ = new QLineEdit();
    cuffEdit_->setMinimumHeight(40);
    cuffEdit_->setPlaceholderText("e.g., Cuffless, Cuffed");
    cuffLayout->addWidget(cuffLabel);
    cuffLayout->addWidget(cuffEdit_);
    inputLayout->addLayout(cuffLayout);
    
    // Reorder number
    QHBoxLayout* reorderLayout = new QHBoxLayout();
    QLabel* reorderLabel = new QLabel("Re-order #:");
    reorderLabel->setMinimumWidth(120);
    reorderEdit_ = new QLineEdit();
    reorderEdit_->setMinimumHeight(40);
    reorderEdit_->setPlaceholderText("Product code");
    reorderLayout->addWidget(reorderLabel);
    reorderLayout->addWidget(reorderEdit_);
    inputLayout->addLayout(reorderLayout);
    
    mainLayout->addWidget(inputGroup_);
    
    // Separator line
    separator_ = new QFrame();
    separator_->setFrameShape(QFrame::HLine);
    separator_->setFrameShadow(QFrame::Sunken);
    separator_->setStyleSheet("QFrame { color: #CCCCCC; }");
    mainLayout->addWidget(separator_);
    
    // Calculated fields section - vertical layout
    calculatedGroup_ = new QGroupBox("Calculated Specifications");
    QVBoxLayout* calcLayout = new QVBoxLayout(calculatedGroup_);
    calcLayout->setSpacing(10);
    
    // Inner Diameter
    QHBoxLayout* idLayout = new QHBoxLayout();
    QLabel* idLabel = new QLabel("Inner Diameter:");
    innerDiameterLabel_ = new QLabel("—");
    innerDiameterLabel_->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; color: #2E7D32; }");
    idLayout->addWidget(idLabel);
    idLayout->addStretch();
    idLayout->addWidget(innerDiameterLabel_);
    calcLayout->addLayout(idLayout);
    
    // Outer Diameter
    QHBoxLayout* odLayout = new QHBoxLayout();
    QLabel* odLabel = new QLabel("Outer Diameter:");
    outerDiameterLabel_ = new QLabel("—");
    outerDiameterLabel_->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; color: #2E7D32; }");
    odLayout->addWidget(odLabel);
    odLayout->addStretch();
    odLayout->addWidget(outerDiameterLabel_);
    calcLayout->addLayout(odLayout);
    
    // Length
    QHBoxLayout* lengthCalcLayout = new QHBoxLayout();
    QLabel* lengthLabel = new QLabel("Length:");
    lengthLabel_ = new QLabel("—");
    lengthLabel_->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; color: #2E7D32; }");
    lengthCalcLayout->addWidget(lengthLabel);
    lengthCalcLayout->addStretch();
    lengthCalcLayout->addWidget(lengthLabel_);
    calcLayout->addLayout(lengthCalcLayout);
    
    // Recommended Suction Catheter
    QHBoxLayout* suctionLayout = new QHBoxLayout();
    QLabel* suctionLabel = new QLabel("Suction Catheter:");
    suctionCatheterLabel_ = new QLabel("—");
    suctionCatheterLabel_->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; color: #1976D2; }");
    suctionLayout->addWidget(suctionLabel);
    suctionLayout->addStretch();
    suctionLayout->addWidget(suctionCatheterLabel_);
    calcLayout->addLayout(suctionLayout);
    
    mainLayout->addWidget(calculatedGroup_);
    
    setLayout(mainLayout);
}

void TubeSpecificationWidget::setupStyling()
{
    StyleManager& styleManager = StyleManager::instance();
    
    // Group box styling
    inputGroup_->setFont(styleManager.getGroupBoxFont());
    calculatedGroup_->setFont(styleManager.getGroupBoxFont());
    
    // Input field styling
    manufacturerCombo_->setFont(styleManager.getFormInputFont());
    sizeCombo_->setFont(styleManager.getFormInputFont());
    typeEdit_->setFont(styleManager.getFormInputFont());
    cuffEdit_->setFont(styleManager.getFormInputFont());
    lengthEdit_->setFont(styleManager.getFormInputFont());
    reorderEdit_->setFont(styleManager.getFormInputFont());
    
    // Apply modern styling
    QString inputStyle = 
        "QComboBox, QLineEdit {"
        "   border: 2px solid #E0E0E0;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "   background-color: white;"
        "}"
        "QComboBox:focus, QLineEdit:focus {"
        "   border-color: #1976D2;"
        "   background-color: #F3F9FF;"
        "}"
        "QComboBox:disabled, QLineEdit:disabled {"
        "   background-color: #F5F5F5;"
        "   color: #9E9E9E;"
        "}";
    
    manufacturerCombo_->setStyleSheet(inputStyle);
    sizeCombo_->setStyleSheet(inputStyle);
    typeEdit_->setStyleSheet(inputStyle);
    cuffEdit_->setStyleSheet(inputStyle);
    reorderEdit_->setStyleSheet(inputStyle);
    
    // Group box styling
    QString groupStyle = 
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #CCCCCC;"
        "   border-radius: 8px;"
        "   margin-top: 1ex;"
        "   padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 8px 0 8px;"
        "   color: #333333;"
        "}";
        
    inputGroup_->setStyleSheet(groupStyle);
    calculatedGroup_->setStyleSheet(groupStyle);
}

void TubeSpecificationWidget::connectSignals()
{
    connect(manufacturerCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TubeSpecificationWidget::onManufacturerChanged);
    connect(sizeCombo_, &QComboBox::currentTextChanged,
            this, &TubeSpecificationWidget::onSizeChanged);
    connect(typeEdit_, &QLineEdit::textChanged, this, &TubeSpecificationWidget::onFieldChanged);
    connect(cuffEdit_, &QLineEdit::textChanged, this, &TubeSpecificationWidget::onFieldChanged);
    connect(lengthEdit_, &QLineEdit::textChanged, this, &TubeSpecificationWidget::onFieldChanged);
    connect(reorderEdit_, &QLineEdit::textChanged, this, &TubeSpecificationWidget::onFieldChanged);
}

void TubeSpecificationWidget::populateManufacturerCombo()
{
    manufacturerCombo_->clear();
    manufacturerCombo_->addItem("Select Manufacturer", "");
    manufacturerCombo_->addItem("Bivona/ICU Medical", "Bivona");
    manufacturerCombo_->addItem("Shiley/Medtronic", "Shiley");
    manufacturerCombo_->addItem("Tracoe/Atos Medical", "Tracoe");
    manufacturerCombo_->addItem("Other/Custom", "Custom");
}

void TubeSpecificationWidget::populateSizeComboForManufacturer(const QString& manufacturer)
{
    updating_ = true;
    
    sizeCombo_->clear();
    
    if (manufacturer.isEmpty()) {
        sizeCombo_->addItem("Select size...");
        updating_ = false;
        return;
    }
    
    // Add common sizes based on manufacturer
    QStringList sizes;
    if (manufacturer == "Bivona") {
        sizes = SATrachTube::BivPedTrachSizes; // Full range
    } else if (manufacturer == "Shiley") {
        sizes = SATrachTube::ShiPedTrachSizes;
        sizes.append(SATrachTube::ShiPedXExtraLongTrachSizes); // Add extended sizes
    } else if (manufacturer == "Tracoe") {
        sizes = SATrachTube::TracPedTrachSizes;
    } else {
        // Custom - add standard range
        sizes = SATrachTube::TrachSizes;
    }
    
    // Remove duplicates and sort
    sizes.removeDuplicates();
    sizes.sort();
    
    sizeCombo_->addItems(sizes);
    sizeCombo_->setCurrentIndex(-1);
    
    updating_ = false;
}

void TubeSpecificationWidget::onManufacturerChanged()
{
    if (updating_) return;
    
    QString manufacturer = manufacturerCombo_->currentData().toString();
    populateSizeComboForManufacturer(manufacturer);
    updateCalculatedFields();
    
    emit manufacturerChanged(manufacturer);
    emit specificationChanged();
}

void TubeSpecificationWidget::onSizeChanged()
{
    if (updating_) return;
    
    updateCalculatedFields();
    
    emit sizeChanged(sizeCombo_->currentText());
    emit specificationChanged();
}

void TubeSpecificationWidget::onFieldChanged()
{
    if (updating_) return;
    
    // Update calculated fields (especially length display)
    updateCalculatedFields();
    
    emit specificationChanged();
}

void TubeSpecificationWidget::updateCalculatedFields()
{
    QString manufacturer = manufacturerCombo_->currentData().toString();
    QString sizeText = sizeCombo_->currentText();
    
    if (manufacturer.isEmpty() || sizeText.isEmpty()) {
        innerDiameterLabel_->setText("—");
        outerDiameterLabel_->setText("—");
        lengthLabel_->setText("—");
        suctionCatheterLabel_->setText("—");
        return;
    }
    
    bool ok;
    double size = sizeText.toDouble(&ok);
    if (!ok) {
        innerDiameterLabel_->setText("—");
        outerDiameterLabel_->setText("—");
        lengthLabel_->setText("—");
        suctionCatheterLabel_->setText("—");
        return;
    }
    
    // Calculate values using the sizing algorithms
    double outerDiameter = SATrachTube::getOuterDiameter(manufacturer, size);
    int suctionCatheter = SATrachTube::getSuctionCatheterSize(size);
    
    // Inner diameter is typically the size itself
    innerDiameterLabel_->setText(QString("%1 mm").arg(size, 0, 'f', 1));
    
    if (outerDiameter > 0) {
        outerDiameterLabel_->setText(QString("%1 mm").arg(outerDiameter, 0, 'f', 1));
    } else {
        outerDiameterLabel_->setText("Not available");
    }
    
    // Length is now manually entered, display the entered value
    if (!lengthEdit_->text().isEmpty()) {
        bool ok;
        double length = lengthEdit_->text().toDouble(&ok);
        if (ok && length > 0) {
            lengthLabel_->setText(QString("%1 mm").arg(length, 0, 'f', 1));
        } else {
            lengthLabel_->setText("Invalid length");
        }
    } else {
        lengthLabel_->setText("—");
    }
    
    if (suctionCatheter > 0) {
        suctionCatheterLabel_->setText(QString("%1 Fr").arg(suctionCatheter));
    } else {
        suctionCatheterLabel_->setText("Not available");
    }
}

void TubeSpecificationWidget::setSpecification(const SpecificationTableRow& spec)
{
    updating_ = true;
    
    // Find manufacturer in combo box
    for (int i = 0; i < manufacturerCombo_->count(); ++i) {
        if (manufacturerCombo_->itemData(i).toString().contains(spec.makeModel, Qt::CaseInsensitive) ||
            spec.makeModel.contains(manufacturerCombo_->itemData(i).toString(), Qt::CaseInsensitive)) {
            manufacturerCombo_->setCurrentIndex(i);
            populateSizeComboForManufacturer(manufacturerCombo_->currentData().toString());
            break;
        }
    }
    
    sizeCombo_->setCurrentText(spec.size);
    typeEdit_->setText(spec.type);
    cuffEdit_->setText(spec.cuff);
    
    if (spec.length > 0) {
        lengthEdit_->setText(QString::number(spec.length));
    } else {
        lengthEdit_->clear();
    }
    
    if (spec.reorderNumber > 0) {
        reorderEdit_->setText(QString::number(spec.reorderNumber));
    } else {
        reorderEdit_->clear();
    }
    
    updating_ = false;
    updateCalculatedFields();
}

SpecificationTableRow TubeSpecificationWidget::getSpecification() const
{
    SpecificationTableRow spec;
    
    spec.makeModel = manufacturerCombo_->currentText();
    spec.size = sizeCombo_->currentText();
    spec.type = typeEdit_->text();
    spec.cuff = cuffEdit_->text();
    
    // Get length from manual input
    spec.length = lengthEdit_->text().toDouble();
    
    // Get calculated values
    QString sizeText = sizeCombo_->currentText();
    bool ok;
    double size = sizeText.toDouble(&ok);
    if (ok) {
        spec.innerDiameter = size;
        QString manufacturer = manufacturerCombo_->currentData().toString();
        spec.outerDiameter = SATrachTube::getOuterDiameter(manufacturer, size);
    }
    
    spec.reorderNumber = reorderEdit_->text().toInt();
    
    return spec;
}

void TubeSpecificationWidget::clear()
{
    updating_ = true;
    
    manufacturerCombo_->setCurrentIndex(0);
    sizeCombo_->clear();
    sizeCombo_->addItem("Select size...");
    typeEdit_->clear();
    cuffEdit_->clear();
    lengthEdit_->clear();
    reorderEdit_->clear();
    
    innerDiameterLabel_->setText("—");
    outerDiameterLabel_->setText("—");
    lengthLabel_->setText("—");
    suctionCatheterLabel_->setText("—");
    
    updating_ = false;
}

void TubeSpecificationWidget::setReadOnly(bool readOnly)
{
    readOnly_ = readOnly;
    
    manufacturerCombo_->setEnabled(!readOnly);
    sizeCombo_->setEnabled(!readOnly);
    typeEdit_->setReadOnly(readOnly);
    cuffEdit_->setReadOnly(readOnly);
    lengthEdit_->setReadOnly(readOnly);
    reorderEdit_->setReadOnly(readOnly);
}

bool TubeSpecificationWidget::isValid() const
{
    return !manufacturerCombo_->currentData().toString().isEmpty() &&
           !sizeCombo_->currentText().isEmpty();
}

QStringList TubeSpecificationWidget::getValidationErrors() const
{
    QStringList errors;
    
    if (manufacturerCombo_->currentData().toString().isEmpty()) {
        errors << "Please select a manufacturer";
    }
    
    if (sizeCombo_->currentText().isEmpty()) {
        errors << "Please select a tube size";
    } else {
        bool ok;
        sizeCombo_->currentText().toDouble(&ok);
        if (!ok) {
            errors << "Please enter a valid tube size";
        }
    }
    
    return errors;
}