#ifndef PATIENTINFOWIDGET_H
#define PATIENTINFOWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "models/Case.h"

class PatientInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInfoWidget(QWidget* parent = nullptr);
    
    void setPatientInfo(const PatientInfo& patient);
    PatientInfo getPatientInfo() const;
    
    void setFrozen(bool frozen);
    bool isFrozen() const { return frozen_; }
    
    void clear();
    
signals:
    void patientInfoChanged();

private slots:
    void onFieldChanged();

private:
    QLineEdit* firstNameEdit_;
    QLineEdit* lastNameEdit_;
    QLineEdit* mrnEdit_;
    QLineEdit* dobEdit_;
    
    QLabel* firstNameLabel_;
    QLabel* lastNameLabel_;
    QLabel* mrnLabel_;
    QLabel* dobLabel_;
    
    bool frozen_;
    
    void setupUI();
    void connectSignals();
    void updateStyles();
};

#endif // PATIENTINFOWIDGET_H