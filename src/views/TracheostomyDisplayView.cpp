#include "TracheostomyDisplayView.h"
#include <QGroupBox>

TracheostomyDisplayView::TracheostomyDisplayView(QWidget* parent)
    : BaseDisplayView(CaseType::Tracheostomy, parent)
    , trachIndicationLabel_(nullptr)
    , suctionInfoLabel_(nullptr)
{
}

void TracheostomyDisplayView::setupCaseSpecificFields()
{
    // Tracheostomy indication
    QGroupBox* indicationGroup = new QGroupBox("Tracheostomy Indication");
    QVBoxLayout* indicationLayout = new QVBoxLayout(indicationGroup);

    trachIndicationLabel_ = new QLabel();
    trachIndicationLabel_->setAlignment(Qt::AlignCenter);
    trachIndicationLabel_->setStyleSheet("font-size: 32px; font-weight: bold; color: #333; padding: 15px;");
    trachIndicationLabel_->setWordWrap(true);

    indicationLayout->addWidget(trachIndicationLabel_);

    // Suction information
    QGroupBox* suctionGroup = new QGroupBox("Suction Information");
    QVBoxLayout* suctionLayout = new QVBoxLayout(suctionGroup);

    suctionInfoLabel_ = new QLabel();
    suctionInfoLabel_->setAlignment(Qt::AlignCenter);
    suctionInfoLabel_->setStyleSheet("font-size: 28px; font-weight: bold; color: #0066CC; padding: 15px;");
    suctionInfoLabel_->setWordWrap(true);

    suctionLayout->addWidget(suctionInfoLabel_);

    specificContentLayout_->addWidget(indicationGroup);
    specificContentLayout_->addWidget(suctionGroup);
    specificContentLayout_->addStretch();
}

void TracheostomyDisplayView::updateDisplay()
{
    // Call base class implementation first
    BaseDisplayView::updateDisplay();

    // Update tracheostomy-specific fields
    trachIndicationLabel_->setText(currentCase_.getTrachIndication());

    SuctionInfo suction = currentCase_.getSuction();
    QString suctionText = QString("Suction Catheter: %1 Fr\nSuction Depth: %2 cm")
                          .arg(suction.size)
                          .arg(suction.depth);
    suctionInfoLabel_->setText(suctionText);
}