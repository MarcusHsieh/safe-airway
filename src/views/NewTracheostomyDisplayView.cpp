#include "NewTracheostomyDisplayView.h"
#include <QGroupBox>

NewTracheostomyDisplayView::NewTracheostomyDisplayView(QWidget* parent)
    : BaseDisplayView(CaseType::NewTracheostomy, parent)
    , procedureLabel_(nullptr)
    , extubationLabel_(nullptr)
    , suctionInfoLabel_(nullptr)
{
}

void NewTracheostomyDisplayView::setupCaseSpecificFields()
{
    // Procedure information
    QGroupBox* procedureGroup = new QGroupBox("Procedure Information");
    QVBoxLayout* procedureLayout = new QVBoxLayout(procedureGroup);

    procedureLabel_ = new QLabel();
    procedureLabel_->setAlignment(Qt::AlignCenter);
    procedureLabel_->setStyleSheet("font-size: 32px; font-weight: bold; color: #333; padding: 15px;");
    procedureLabel_->setWordWrap(true);

    procedureLayout->addWidget(procedureLabel_);

    // Extubation information
    QGroupBox* extubationGroup = new QGroupBox("Extubation Information");
    QVBoxLayout* extubationLayout = new QVBoxLayout(extubationGroup);

    extubationLabel_ = new QLabel();
    extubationLabel_->setAlignment(Qt::AlignCenter);
    extubationLabel_->setStyleSheet("font-size: 28px; font-weight: bold; color: #FF6600; padding: 15px;");
    extubationLabel_->setWordWrap(true);

    extubationLayout->addWidget(extubationLabel_);

    // Suction information
    QGroupBox* suctionGroup = new QGroupBox("Suction Information");
    QVBoxLayout* suctionLayout = new QVBoxLayout(suctionGroup);

    suctionInfoLabel_ = new QLabel();
    suctionInfoLabel_->setAlignment(Qt::AlignCenter);
    suctionInfoLabel_->setStyleSheet("font-size: 28px; font-weight: bold; color: #0066CC; padding: 15px;");
    suctionInfoLabel_->setWordWrap(true);

    suctionLayout->addWidget(suctionInfoLabel_);

    specificContentLayout_->addWidget(procedureGroup);
    specificContentLayout_->addWidget(extubationGroup);
    specificContentLayout_->addWidget(suctionGroup);
    specificContentLayout_->addStretch();
}

void NewTracheostomyDisplayView::updateDisplay()
{
    // Call base class implementation first
    BaseDisplayView::updateDisplay();

    // Update new tracheostomy-specific fields
    procedureLabel_->setText(currentCase_.getProcedure());

    QString extubationText = QString("Extubation Date: %1").arg(currentCase_.getExtubationDate());
    extubationLabel_->setText(extubationText);

    SuctionInfo suction = currentCase_.getSuction();
    QString suctionText = QString("Suction Catheter: %1 Fr\nSuction Depth: %2 cm")
                          .arg(suction.size)
                          .arg(suction.depth);
    suctionInfoLabel_->setText(suctionText);
}