#ifndef SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H
#define SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H

#pragma once

#include "text_transforms.h"
#include <QMainWindow>
#include <QTextEdit>
#include <memory>
#include <vector>

class main_window : public QMainWindow {
    Q_OBJECT

public:
    main_window();
    ~main_window() override;

private:
    void setup_file_menu();
    void setup_format_menu();
    void apply_transform(const text_transform& transform) const;

    void open_file();
    void save_file();
    void save_file_as();
    void update_title();

    QTextEdit* editor = nullptr;
    QString current_file;
    std::vector<std::unique_ptr<text_transform>>transforms;

};
#endif //SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H
