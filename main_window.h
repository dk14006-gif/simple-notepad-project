#ifndef SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H
#define SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H

#pragma once

#include <QMainWindow>
#include <QTextEdit>

class main_window : public QMainWindow {
    Q_OBJECT

public:
    main_window();
    ~main_window() override;

private:
    QTextEdit* editor = nullptr;
};
#endif //SIMPLE_NOTEPAD_PROJECT_MAIN_WINDOW_H
