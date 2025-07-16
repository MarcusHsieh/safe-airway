#ifndef TEXTWRAPDELEGATE_H
#define TEXTWRAPDELEGATE_H

#include <QStyledItemDelegate>
#include <QVector>

class TextWrapDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TextWrapDelegate(QObject *parent = nullptr);
    
    // Set character limits for each column (column index -> character limit)
    void setColumnCharacterLimits(const QVector<int> &limits);
    
    // Set character limit for special comments (when used outside tables)
    void setSpecialCommentsCharacterLimit(int limit);

    // Override these crucial methods from the base class
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    
    // Override editor methods to implement configurable character wrapping
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    QString processTextForCharWrap(const QString &text, int charLimit) const;
    int getCharacterLimitForColumn(int column) const;
    
    QVector<int> columnCharacterLimits_;
    int specialCommentsCharacterLimit_;
};

#endif // TEXTWRAPDELEGATE_H