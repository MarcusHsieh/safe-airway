#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_tracheostomyButton_clicked();

    void on_difficultAirwayButton_clicked();

    void on_nTracheostomyButton_clicked();

    void on_LTRButton_clicked();

private:
    Ui::MainWindow *ui;

    void resizeEvent(QResizeEvent *event);
    void updateImage();

};
#endif // MAINWINDOW_H
