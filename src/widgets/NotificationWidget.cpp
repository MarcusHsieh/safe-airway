#include "NotificationWidget.h"
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QShowEvent>
#include <QResizeEvent>
#include <QGuiApplication>
#include <QScreen>

NotificationWidget::NotificationWidget(QWidget* parent)
    : QWidget(parent)
    , messageLabel_(nullptr)
    , hideTimer_(nullptr)
    , fadeAnimation_(nullptr)
{
    setupUI();
    setupAnimation();
    
    // Initially hidden
    setVisible(false);
}

void NotificationWidget::setupUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // Create layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 10, 15, 10);
    
    // Create message label
    messageLabel_ = new QLabel();
    messageLabel_->setAlignment(Qt::AlignCenter);
    messageLabel_->setWordWrap(true);
    messageLabel_->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(0, 0, 0, 180);"
        "   color: white;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
    );
    
    layout->addWidget(messageLabel_);
    
    // Create hide timer
    hideTimer_ = new QTimer(this);
    hideTimer_->setSingleShot(true);
    connect(hideTimer_, &QTimer::timeout, this, &NotificationWidget::fadeOut);
}

void NotificationWidget::setupAnimation()
{
    // Create fade animation
    fadeAnimation_ = new QPropertyAnimation(this, "windowOpacity");
    fadeAnimation_->setDuration(300);
    connect(fadeAnimation_, &QPropertyAnimation::finished, this, &NotificationWidget::hideNotification);
}

void NotificationWidget::showNotification(const QString& message, int duration)
{
    messageLabel_->setText(message);
    messageLabel_->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(0, 0, 0, 180);"
        "   color: white;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
    );
    
    positionWidget();
    setWindowOpacity(1.0);
    show();
    raise();
    
    hideTimer_->start(duration);
}

void NotificationWidget::showError(const QString& message, int duration)
{
    messageLabel_->setText(message);
    messageLabel_->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(220, 20, 60, 180);"
        "   color: white;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
    );
    
    positionWidget();
    setWindowOpacity(1.0);
    show();
    raise();
    
    hideTimer_->start(duration);
}

void NotificationWidget::showSuccess(const QString& message, int duration)
{
    messageLabel_->setText(message);
    messageLabel_->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(34, 139, 34, 180);"
        "   color: white;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
    );
    
    positionWidget();
    setWindowOpacity(1.0);
    show();
    raise();
    
    hideTimer_->start(duration);
}

void NotificationWidget::positionWidget()
{
    if (!parent()) return;
    
    QWidget* parentWidget = qobject_cast<QWidget*>(parent());
    if (!parentWidget) return;
    
    // Position in top-right corner of parent
    QSize parentSize = parentWidget->size();
    QSize thisSize = sizeHint();
    
    int x = parentSize.width() - thisSize.width() - 20;
    int y = 20;
    
    // Convert to global coordinates
    QPoint globalPos = parentWidget->mapToGlobal(QPoint(x, y));
    move(globalPos);
}

void NotificationWidget::fadeOut()
{
    fadeAnimation_->setStartValue(1.0);
    fadeAnimation_->setEndValue(0.0);
    fadeAnimation_->start();
}

void NotificationWidget::hideNotification()
{
    if (windowOpacity() <= 0.0) {
        hide();
    }
}

void NotificationWidget::showEvent(QShowEvent* event)
{
    positionWidget();
    QWidget::showEvent(event);
}

void NotificationWidget::resizeEvent(QResizeEvent* event)
{
    positionWidget();
    QWidget::resizeEvent(event);
}