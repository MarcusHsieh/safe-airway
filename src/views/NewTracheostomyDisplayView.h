#ifndef NEWTRACHEOSTOMYDISPLAYVIEW_H
#define NEWTRACHEOSTOMYDISPLAYVIEW_H

#include "BaseDisplayView.h"
#include <QLabel>

class NewTracheostomyDisplayView : public BaseDisplayView
{
    Q_OBJECT

public:
    explicit NewTracheostomyDisplayView(QWidget* parent = nullptr);

protected:
    void setupCaseSpecificFields() override;
    void updateDisplay() override;

private:
    QLabel* procedureLabel_;
    QLabel* extubationLabel_;
    QLabel* suctionInfoLabel_;
};

#endif // NEWTRACHEOSTOMYDISPLAYVIEW_H