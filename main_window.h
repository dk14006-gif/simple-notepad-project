#ifndef SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H
#define SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H

#pragma once
#include "spell_checker.h"
#include "text_transforms.h"
#include "spell_checker_highlighter.h"
#include "line_number_area.h"
#include <QMainWindow>
#include <QTextEdit>
#include <memory>
#include <vector>
#include <QLabel>
#include <QStatusBar>

namespace Ui
{
    class find_replace_dialog;
    class word_frequency_dialog;
}

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window();
    ~main_window() override;

private:
    void setup_file_menu();
    void setup_format_menu();
    void setup_edit_menu();
    void setup_format_toolbar();
    void setup_tools_menu();
    void setup_search_menu();
    void show_word_frequency();
    void setup_status_bar();
    void update_status_bar();
    void choose_font();
    void choose_text_color();
    void zoom_in();
    void setup_view_menu();
    void zoom_out();
    void zoom_reset();
    void update_line_number_area();

    int m_zoom_level = 0;

    void apply_transform(const text_transform& transform) const;
    void show_find_replace_dialog();
    void find_next(const QString& term, QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_current(const QString& term, const QString& replacement,
                         QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_all(const QString& term, const QString& replacement,
                     QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void show_context_menu(const QPoint& pos);


    void open_file();
    void save_file();
    void save_file_as();
    void update_title();

    QTextEdit* editor = nullptr;
    QString current_file;
    std::vector<std::unique_ptr<text_transform>> transforms;
    QDialog* find_replace_dlg = nullptr;
    std::unique_ptr<Ui::find_replace_dialog> find_replace_ui;
    std::unique_ptr<spell_checker> m_spell_checker;
    spell_checker_highlighter* m_highlighter = nullptr;

    QLabel* m_label_words = nullptr;
    QLabel* m_label_lines = nullptr;
    QLabel* m_label_cursor = nullptr;

    line_number_area* m_line_number_area = nullptr;
};
#endif //SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H
