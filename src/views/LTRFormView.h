#ifndef LTRFORMVIEW_H
#define LTRFORMVIEW_H

#include "widgets/BaseFormWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

class LTRFormView : public BaseFormWidget
{
    Q_OBJECT

public:
    explicit LTRFormView(QWidget* parent = nullptr);
    
    void setCase(const Case& case_) override;
    Case getCase() const override;

protected:
    void setupFormSpecificFields() override;

private:
    QGroupBox* additionalFieldsGroup_;
    QLineEdit* procedureEdit_;
    QLineEdit* surgeonEdit_;
    QLineEdit* dateOfSurgeryEdit_;
    QLineEdit* extubationDateEdit_;
    
    void connectAdditionalSignals();
};

#endif // LTRFORMVIEW_H