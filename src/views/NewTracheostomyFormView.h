#ifndef NEWTRACHEOSTOMYFORMVIEW_H
#define NEWTRACHEOSTOMYFORMVIEW_H

#include "widgets/BaseFormWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

class NewTracheostomyFormView : public BaseFormWidget
{
    Q_OBJECT

public:
    explicit NewTracheostomyFormView(QWidget* parent = nullptr);
    
    void setCase(const Case& case_) override;
    Case getCase() const override;

protected:
    void setupFormSpecificFields() override;

private:
    QGroupBox* additionalFieldsGroup_;
    QLineEdit* trachIndicationEdit_;
    QLineEdit* surgeonEdit_;
    QLineEdit* dateOfSurgeryEdit_;
    QLineEdit* firstTrachChangeEdit_;
    
    void connectAdditionalSignals();
};

#endif // NEWTRACHEOSTOMYFORMVIEW_H