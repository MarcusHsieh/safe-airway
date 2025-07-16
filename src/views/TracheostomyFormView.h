#ifndef TRACHEOSTOMYFORMVIEW_H
#define TRACHEOSTOMYFORMVIEW_H

#include "widgets/BaseFormWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

class TracheostomyFormView : public BaseFormWidget
{
    Q_OBJECT

public:
    explicit TracheostomyFormView(QWidget* parent = nullptr);
    
    void setCase(const Case& case_) override;
    Case getCase() const override;

protected:
    void setupFormSpecificFields() override;

private:
    QGroupBox* specificFieldsGroup_;
    QLineEdit* trachIndicationEdit_;
    QLabel* trachIndicationLabel_;
};

#endif // TRACHEOSTOMYFORMVIEW_H