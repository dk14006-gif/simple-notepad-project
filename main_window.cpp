#include "main_window.h"

#include <QTextEdit>

main_window::main_window()
{
    setWindowTitle("Notepad");
    resize(800, 600);
    editor = new QTextEdit(this);
    setCentralWidget(editor);
}

main_window::~main_window() = default;
