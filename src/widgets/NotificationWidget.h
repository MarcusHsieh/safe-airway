#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>

class NotificationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationWidget(QWidget* parent = nullptr);
    
    void showNotification(const QString& message, int duration = 3000);
    void showError(const QString& message, int duration = 5000);
    void showSuccess(const QString& message, int duration = 3000);

private slots:
    void fadeOut();
    void hideNotification();

private:
    QLabel* messageLabel_;
    QTimer* hideTimer_;
    QPropertyAnimation* fadeAnimation_;
    
    void setupUI();
    void setupAnimation();
    void positionWidget();
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
};

#endif // NOTIFICATIONWIDGET_H