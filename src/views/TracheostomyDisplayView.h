#ifndef TRACHEOSTOMYDISPLAYVIEW_H
#define TRACHEOSTOMYDISPLAYVIEW_H

#include "BaseDisplayView.h"
#include <QLabel>

class TracheostomyDisplayView : public BaseDisplayView
{
    Q_OBJECT

public:
    explicit TracheostomyDisplayView(QWidget* parent = nullptr);

protected:
    void setupCaseSpecificFields() override;
    void updateDisplay() override;

private:
    QLabel* trachIndicationLabel_;
    QLabel* suctionInfoLabel_;
};

#endif // TRACHEOSTOMYDISPLAYVIEW_H