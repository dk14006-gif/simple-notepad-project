#include "spell_checker_highlighter.h"

spell_checker_highlighter::spell_checker_highlighter(QTextDocument* parent, spell_checker* checker) :
    QSyntaxHighlighter(parent), m_checker(checker)
{
    m_misspelled_format.setUnderlineColor(Qt::red);
    m_misspelled_format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
}

void spell_checker_highlighter::highlightBlock(const QString& text)
{
    if (!m_checker || !m_checker->is_loaded()) return;

    int start = -1;
    for (int i = 0; i <= text.length(); ++i)
    {
        bool is_letter = (i < text.length()) && text[i].isLetter();

        if (is_letter && start == -1)
        {
            start = i;
        }
        else if (!is_letter && start != -1)
        {
            QString word = text.mid(start, i - start);
            if (!m_checker->is_correct(word.toStdString()))
            {
                setFormat(start, i - start, m_misspelled_format);
            }
            start = -1;
        }
    }
}
