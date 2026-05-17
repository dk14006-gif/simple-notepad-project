#ifndef SIMPLE_NOTEPAD_PROJECT_SPELL_CHECKER_HIGHLIGHTER_H
#define SIMPLE_NOTEPAD_PROJECT_SPELL_CHECKER_HIGHLIGHTER_H

#pragma once

#include "spell_checker.h"
#include <QSyntaxHighlighter>
#include <QTextFormat>

class spell_checker_highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit spell_checker_highlighter(QTextDocument* parent, spell_checker* checker);
protected:
    void highlightBlock(const QString& text) override;

private:
    spell_checker* m_checker;
    QTextCharFormat m_misspelled_format;
};
#endif //SIMPLE_NOTEPAD_PROJECT_SPELL_CHECKER_HIGHLIGHTER_H