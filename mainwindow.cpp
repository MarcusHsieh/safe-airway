#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Get screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    this->showFullScreen();
    // Resize the main window to fit the screen
    this->resize(screenWidth, screenHeight);
    // Set size policies for widgets to expand
    // ui->pushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // ui->pushButton_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // ui->pushButton_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // ui->pushButton_4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->stackedWidget->setCurrentIndex(0);
    // QFont font = this->font();
    // font.setPointSizeF(font.pointSizeF() * devicePixelRatioF());
    // this->setFont(font);
    updateImage();

    connect(ui->cantSuctionRadioButton, &QRadioButton::toggled, this, &MainWindow::updateEmergencyAdvice);
    connect(ui->cantVentilateRadioButton, &QRadioButton::toggled, this, &MainWindow::updateEmergencyAdvice);
    connect(ui->o2SatDropRadioButton, &QRadioButton::toggled, this, &MainWindow::updateEmergencyAdvice);
    connect(ui->decannulationRadioButton, &QRadioButton::toggled, this, &MainWindow::updateEmergencyAdvice);
    connect(ui->hemoptysisRadioButton, &QRadioButton::toggled, this, &MainWindow::updateEmergencyAdvice);
    updateEmergencyAdvice();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tracheostomyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_difficultAirwayButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_nTracheostomyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_LTRButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::updateImage() {
    // QPixmap pixmap("C:/_Projects/safe-airway/images/nemours_logo.png");
    QPixmap pixmap(":/images/nemours_logo.png");
    if (!pixmap.isNull()) {
        ui->logoLabel->setPixmap(pixmap.scaled(
            ui->logoLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            ));
        ui->logoLabel->setScaledContents(true);
    } else {
        qDebug() << "fail";
    }
}



void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateImage();
}

// EMERGENCY //

void MainWindow::updateEmergencyAdvice()
{
    QString advice;

    if (ui->cantSuctionRadioButton->isChecked()) {
        advice = buildCantSuctionAdvice();
    }
    else if (ui->cantVentilateRadioButton->isChecked()) {
        advice = buildCantVentilateAdvice();
    }
    else if (ui->o2SatDropRadioButton->isChecked()) {
        advice = buildO2SatDropAdvice();
    }
    else if (ui->decannulationRadioButton->isChecked()) {
        advice = buildDecannulationAdvice();
    }
    else if (ui->hemoptysisRadioButton->isChecked()) {
        advice = buildHemoptysisAdvice();
    }
    else {
        advice = "No emergency scenario selected. Please select one when needed.";
    }

    ui->precautionsTextBox->setText(advice);
}


// string build logic //

QString MainWindow::buildCantSuctionAdvice()
{
    // "Can't Suction"
    QString advice;
    advice.append("1. Contact the surgeon on call for this patient at: " + _onCallPhoneNumber + ".\n\n");
    advice.append("2. Prepare for flexible fiberoptic airway examination at the bedside.\n\n");
    advice.append("3. Wait for the on-call team for additional assistance.\n");
    return advice;
}

QString MainWindow::buildCantVentilateAdvice()
{
    // "Can't Ventilate"
    QString advice;
    advice.append("1. Begin CPR.\n\n");
    advice.append("2. Contact the surgeon on call for this patient at: " + _onCallPhoneNumber + ".\n\n");
    advice.append("3. Replace the tracheostomy tube with a " + _ettSize + " endotracheal tube and attempt ventilation.\n\n");
    advice.append("4. If ventilation fails, remove the endotracheal tube and attempt bag-mask ventilation.\n\n");
    advice.append("5. Wait for the on-call team for additional assistance.\n");
    return advice;
}

QString MainWindow::buildO2SatDropAdvice()
{
    // "O2 Sat Drop"
    QString advice;
    // Assuming a stable scenario; adjust as needed for unstable conditions
    advice.append("1. Increase FIO2 to 100%.\n\n");
    advice.append("2. Contact the on-call team at: " + _onCallPhoneNumber + ".\n\n");
    advice.append("3. Setup the endoscopic airway cart at the bedside.\n\n");
    advice.append("4. Request a STAT portable chest x-ray.\n\n");
    advice.append("5. Wait for the on-call team to arrive.\n");
    return advice;
}

QString MainWindow::buildDecannulationAdvice()
{
    // "Decannulation"
    QString advice;
    advice.append("1. Contact the surgeon on call for this patient at: " + _onCallPhoneNumber + ".\n\n");
    advice.append("2. Remove the tracheostomy tube and attempt oral intubation with a " + _ettSize + " endotracheal tube.\n\n");
    advice.append("3. Prepare a replacement tracheostomy tube if available.\n\n");
    advice.append("4. Wait for the on-call team for additional assistance.\n");
    return advice;
}

QString MainWindow::buildHemoptysisAdvice()
{
    // "Hemoptysis"
    QString advice;
    advice.append("1. If the patient is not breathing, begin CPR.\n\n");
    advice.append("2. Contact the surgeon on call for this patient at: " + _onCallPhoneNumber + ".\n\n");
    advice.append("3. If a cuffed tracheostomy tube is in place, inflate the cuff; otherwise, replace with a cuffed tube.\n\n");
    advice.append("4. Instill saline into the tube lumen and suction with a catheter.\n\n");
    advice.append("5. Prepare for a bedside flexible endoscopic evaluation of the airway.\n\n");
    advice.append("6. Notify radiology for a STAT CT-Angiogram of the chest and neck.\n\n");
    advice.append("7. Draw blood for CBC and Type & Crossmatch.\n");
    return advice;
}


void MainWindow::on_saveExitButton_clicked()
{
    // add saving logic here, implement database
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_cancelButton_clicked()
{
    // doesn't save, exits immediately
    ui->stackedWidget->setCurrentIndex(0);
}

