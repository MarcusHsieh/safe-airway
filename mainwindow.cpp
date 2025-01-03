#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Get the screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    this->showFullScreen();
    // Resize the main window to fit the screen
    this->resize(screenWidth, screenHeight);
    // Set size policies for widgets to expand
    ui->pushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pushButton_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pushButton_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pushButton_4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->stackedWidget->setCurrentIndex(0);
    // QFont font = this->font();
    // font.setPointSizeF(font.pointSizeF() * devicePixelRatioF());
    // this->setFont(font);
    updateImage();
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

// void MainWindow::updateImage() {
//     // load pixmap
//     // QPixmap pixmap(":/images/nemours_logo");
//     QPixmap pixmap("C:/_Projects/safe-airway/nemours_logo.png");
//     if (pixmap.isNull()) {
//         qDebug() << "fail";
//         return;
//     }
//     qDebug() << "loaded, size = " << pixmap.size();

//     // set button icon
//     QIcon icon(pixmap);
//     if (icon.isNull()) {
//         qDebug() << "fail ";
//         return;
//     }

//     ui->logoButton->setIcon(icon);
//     // ui->logoButton->setIconSize(QSize(50, 50));
//     ui->logoButton->setFlat(true);
//     qDebug() << "good";
// }

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
