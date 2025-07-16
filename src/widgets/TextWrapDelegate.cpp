#include "TextWrapDelegate.h"
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QTextEdit>
#include <QTableWidget>

TextWrapDelegate::TextWrapDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , specialCommentsCharacterLimit_(67) // Default to 67 characters
{
}

void TextWrapDelegate::setColumnCharacterLimits(const QVector<int> &limits)
{
    columnCharacterLimits_ = limits;
}

void TextWrapDelegate::setSpecialCommentsCharacterLimit(int limit)
{
    specialCommentsCharacterLimit_ = limit;
}

void TextWrapDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Ensure we have a valid state to paint in
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    QString text = index.data(Qt::DisplayRole).toString();
    if (text.isEmpty()) {
        return;
    }

    painter->save();

    QTextDocument doc;
    doc.setTextWidth(option.rect.width()); // This is the key to wrapping!
    doc.setPlainText(text);

    // Position the text within the cell, accounting for a small margin
    painter->translate(option.rect.topLeft() + QPoint(5, 5)); // Add 5px margin
    
    // Set the text color
    QAbstractTextDocumentLayout::PaintContext ctx;
    if (option.state & QStyle::State_Selected) {
        ctx.palette.setColor(QPalette::Text, option.palette.highlightedText().color());
    } else {
        ctx.palette.setColor(QPalette::Text, option.palette.text().color());
    }
    
    doc.documentLayout()->draw(painter, ctx);

    painter->restore();
}

QSize TextWrapDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    
    // Get character limit for this column
    int charLimit = getCharacterLimitForColumn(index.column());
    
    // Process text to wrap at the specified character limit for size calculation
    QString processedText = processTextForCharWrap(text, charLimit);

    QTextDocument doc;
    doc.setPlainText(processedText);
    doc.setTextWidth(option.rect.width()); // Use the cell's width to calculate height
    
    // Return the required size with a bit of vertical padding
    return QSize(doc.idealWidth(), doc.size().height() + 10);
}

QWidget *TextWrapDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    
    QTextEdit *editor = new QTextEdit(parent);
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setWordWrapMode(QTextOption::WordWrap);
    editor->setLineWrapMode(QTextEdit::WidgetWidth);
    
    // Get character limit for this column
    int charLimit = getCharacterLimitForColumn(index.column());
    
    // Connect to implement configurable character wrapping
    connect(editor, &QTextEdit::textChanged, [this, editor, charLimit]() {
        // Block signals to prevent recursion
        editor->blockSignals(true);
        
        QTextCursor cursor = editor->textCursor();
        QString text = editor->toPlainText();
        
        // Process text for character wrapping
        QString processedText = this->processTextForCharWrap(text, charLimit);
        
        if (processedText != text) {
            editor->setPlainText(processedText);
            // Set cursor to end of text
            cursor.setPosition(processedText.length());
            editor->setTextCursor(cursor);
        }
        
        editor->blockSignals(false);
        
        // Resize editor height based on content
        QTextDocument *doc = editor->document();
        int docHeight = static_cast<int>(doc->size().height()) + 20;
        int minHeight = 80;
        int maxHeight = 300;
        int newHeight = qMax(minHeight, qMin(maxHeight, docHeight));
        editor->setFixedHeight(newHeight);
        
        // Notify parent table to resize row
        if (QTableWidget* table = qobject_cast<QTableWidget*>(editor->parent()->parent())) {
            table->resizeRowsToContents();
        }
    });
    
    return editor;
}

void TextWrapDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(editor);
    if (textEdit) {
        textEdit->setPlainText(value);
    }
}

void TextWrapDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(editor);
    if (textEdit) {
        QString value = textEdit->toPlainText();
        model->setData(index, value, Qt::EditRole);
    }
}

QString TextWrapDelegate::processTextForCharWrap(const QString &text, int charLimit) const
{
    // Split text into lines and process each line
    QStringList lines = text.split('\n');
    QStringList processedLines;
    
    for (const QString& line : lines) {
        if (line.length() <= charLimit) {
            processedLines << line;
        } else {
            // Split long lines at charLimit characters, preferring word boundaries
            QString remainingText = line;
            while (remainingText.length() > charLimit) {
                int breakPoint = charLimit;
                // Try to break at a space near the character limit (search back up to 17 characters)
                int searchStart = qMax(charLimit - 17, charLimit / 2);
                for (int i = charLimit; i >= searchStart; --i) {
                    if (i < remainingText.length() && remainingText[i] == ' ') {
                        breakPoint = i;
                        break;
                    }
                }
                
                processedLines << remainingText.left(breakPoint);
                remainingText = remainingText.mid(breakPoint).trimmed();
            }
            if (!remainingText.isEmpty()) {
                processedLines << remainingText;
            }
        }
    }
    
    return processedLines.join('\n');
}

int TextWrapDelegate::getCharacterLimitForColumn(int column) const
{
    // Return the character limit for the specified column, or default if not set
    if (column >= 0 && column < columnCharacterLimits_.size()) {
        return columnCharacterLimits_[column];
    }
    
    // Default character limits based on typical column content
    switch (column) {
        case 0: return 30; // Make/Model
        case 1: return 10; // Size
        case 2: return 20; // Type
        case 3: return 10; // Cuff
        case 4: return 15; // Inner Diameter
        case 5: return 15; // Outer Diameter
        case 6: return 15; // Length
        case 7: return 20; // Re-order #
        default: return 25; // Generic fallback
    }
}