#ifndef ESCOVERLAYMENU_H
#define ESCOVERLAYMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>

class EscOverlayMenu : public QWidget
{
    Q_OBJECT

public:
    explicit EscOverlayMenu(QWidget* parent = nullptr);
    
    void showMenu();
    void hideMenu();
    bool isMenuVisible() const;

signals:
    void newCaseRequested();
    void openCaseRequested();
    void saveRequested();
    void saveAsRequested();
    void increaseFontRequested();
    void decreaseFontRequested();
    void resetFontRequested();
    void aboutRequested();
    void exitRequested();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onNewCaseClicked();
    void onOpenCaseClicked();
    void onSaveClicked();
    void onSaveAsClicked();
    void onIncreaseFontClicked();
    void onDecreaseFontClicked();
    void onResetFontClicked();
    void onAboutClicked();
    void onExitClicked();

private:
    void setupUI();
    void setupButtonStyling();
    QString getButtonStyleSheet(const QString& color, bool isFileButton = false) const;
    
    QWidget* menuPanel_;
    QGridLayout* buttonLayout_;
    
    QPushButton* newCaseButton_;
    QPushButton* openCaseButton_;
    QPushButton* saveButton_;
    QPushButton* saveAsButton_;
    QPushButton* increaseFontButton_;
    QPushButton* decreaseFontButton_;
    QPushButton* resetFontButton_;
    QPushButton* aboutButton_;
    QPushButton* exitButton_;
    
    QLabel* titleLabel_;
    
    bool visible_;
};

#endif // ESCOVERLAYMENU_H