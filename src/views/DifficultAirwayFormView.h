#ifndef DIFFICULTAIRWAYFORMVIEW_H
#define DIFFICULTAIRWAYFORMVIEW_H

#include "widgets/BaseFormWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

class DifficultAirwayFormView : public BaseFormWidget
{
    Q_OBJECT

public:
    explicit DifficultAirwayFormView(QWidget* parent = nullptr);
    
    void setCase(const Case& case_) override;
    Case getCase() const override;

protected:
    void setupFormSpecificFields() override;

private:
    QGroupBox* additionalFieldsGroup_;
    QLineEdit* airwayDiagnosisEdit_;
    
    void connectAdditionalSignals();
};

#endif // DIFFICULTAIRWAYFORMVIEW_H