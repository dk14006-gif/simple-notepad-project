#ifndef SIMPLE_NOTEPAD_PROJECT_LINE_NUMBER_AREA_H
#define SIMPLE_NOTEPAD_PROJECT_LINE_NUMBER_AREA_H

#pragma once

#include <QTextEdit>
#include <QWidget>

class line_number_area : public QWidget
{
    Q_OBJECT;
public:
    explicit line_number_area(QTextEdit* editor);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QTextEdit* m_editor;
    int line_number_width() const;
};
#endif //SIMPLE_NOTEPAD_PROJECT_LINE_NUMBER_AREA_H