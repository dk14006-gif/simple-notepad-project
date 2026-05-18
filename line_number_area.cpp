#include "line_number_area.h"

#include <QTextBlock>
#include <QPainter>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>

line_number_area::line_number_area(QTextEdit* editor)
    : QWidget(editor->parentWidget()), m_editor(editor)
{
};

int line_number_area::line_number_width() const
{
    int digits = 1;
    int max = qMax(1, m_editor->document()->blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    return 6 + fontMetrics().horizontalAdvance('9') * digits;
}

QSize line_number_area::sizeHint() const
{
    return {line_number_width(), 0};
}

void line_number_area::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), QColor{240, 240, 240});

    const int offset = m_editor->verticalScrollBar()->value();
    const QFontMetrics fm(m_editor->font());
    const int line_height = fm.height();

    QTextBlock block = m_editor->document()->begin();
    int block_number = 0;
    int top = static_cast<int>(
        m_editor->document()->documentLayout()->blockBoundingRect(block).translated(0, -offset).top());
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && top >= event->rect().top() - line_height)
        {
            painter.setPen(QColor{130, 130, 130});
            painter.drawText(0, top, width() - 3, line_height,
                             Qt::AlignRight, QString::number(block_number + 1));
        }
        block = block.next();
        ++block_number;
        top = static_cast<int>(
            m_editor->document()->documentLayout()->blockBoundingRect(block).translated(0, -offset).top());
    }
}
