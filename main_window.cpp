#include "main_window.h"

#include <QTextEdit>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>

main_window::main_window()
{
    setWindowTitle("Notepad");
    resize(800, 600);
    editor = new QTextEdit(this);
    setCentralWidget(editor);

    transforms.push_back(std::make_unique<uppercase_transform>());
    transforms.push_back(std::make_unique<lowercase_transform>());
    transforms.push_back(std::make_unique<capitalize_transform>());
    transforms.push_back(std::make_unique<sentence_case_transform>());
    transforms.push_back(std::make_unique<swap_case_transform>());

    setup_file_menu();
    setup_edit_menu();
    setup_format_menu();
    setup_format_toolbar();
}

main_window::~main_window() = default;

void main_window::setup_file_menu()
{
    auto* file_menu = menuBar()->addMenu("File");

    const auto* action_new = file_menu->addAction("New");
    connect(action_new, &QAction::triggered, this, [this]
    {
        editor->clear();
        current_file.clear();
        update_title();
    });

    file_menu->addSeparator();

    const auto* action_open = file_menu->addAction("Open...");
    connect(action_open, &QAction::triggered, this, [this]
    {
        open_file();
    });

    const auto* action_save = file_menu->addAction("Save");
    connect(action_save, &QAction::triggered, this, [this]
    {
        save_file();
    });

    const auto* action_save_as = file_menu->addAction("Save As...");
    connect(action_save_as, &QAction::triggered, this, [this]
    {
        save_file_as();
    });

    file_menu->addSeparator();

    const auto* action_exit = file_menu->addAction("Exit");
    connect(action_exit, &QAction::triggered, this, []
    {
        QApplication::quit();
    });
}

void main_window::open_file()
{
    const auto path = QFileDialog::getOpenFileName(this, "Open File");
    if (path.isEmpty())
    {
        return;
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    editor->setPlainText(in.readAll());
    current_file = path;
    update_title();
}

void main_window::save_file()
{
    if (current_file.isEmpty())
    {
        save_file_as();
        return;
    }
    QFile file(current_file);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    out << editor->toPlainText();
}

void main_window::save_file_as()
{
    const auto path = QFileDialog::getSaveFileName(this, "Save File As");
    if (path.isEmpty())
    {
        return;
    }
    current_file = path;
    save_file();
    update_title();
}

void main_window::update_title()
{
    if (current_file.isEmpty())
    {
        setWindowTitle("Notepad");
    }
    else
    {
        setWindowTitle("Notepad: " + current_file);
    }
}

void main_window::setup_edit_menu()
{
    auto* edit_menu = menuBar()->addMenu("Edit");

    auto* action_undo = edit_menu->addAction("Undo");
    action_undo->setShortcut(QKeySequence::Undo);
    connect(action_undo, &QAction::triggered, editor, &QTextEdit::undo);

    auto* action_redo = edit_menu->addAction("Redo");
    action_redo->setShortcut(QKeySequence::Redo);
    connect(action_redo, &QAction::triggered, editor, &QTextEdit::redo);

    edit_menu->addSeparator();

    auto* action_cut = edit_menu->addAction("Cut");
    action_cut->setShortcut(QKeySequence::Cut);
    connect(action_cut, &QAction::triggered, editor, &QTextEdit::cut);

    auto* action_copy = edit_menu->addAction("Copy");
    action_copy->setShortcut(QKeySequence::Copy);
    connect(action_copy, &QAction::triggered, editor, &QTextEdit::copy);

    auto* action_paste = edit_menu->addAction("Paste");
    action_paste->setShortcut(QKeySequence::Paste);
    connect(action_paste, &QAction::triggered, editor, &QTextEdit::paste);

    edit_menu->addSeparator();

    auto* action_select_all = edit_menu->addAction("Select All");
    action_select_all->setShortcut(QKeySequence::SelectAll);
    connect(action_select_all, &QAction::triggered, editor, &QTextEdit::selectAll);
}

void main_window::setup_format_menu()
{
    auto* format_menu = menuBar()->addMenu("Format");
    auto* text_case_menu = format_menu->addMenu("Text Case");

    for (const auto& transform : transforms)
    {
        const auto* action = text_case_menu->addAction(QString::fromStdString(transform->name()));
        connect(action, &QAction::triggered, this, [this, &transform]
        {
            apply_transform(*transform);
        });
    }
}

void main_window::apply_transform(const text_transform& transform) const
{
    auto cursor = editor->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::Document);
    }
    const int start = cursor.selectionStart();
    const QString selected = cursor.selectedText().replace(QChar::ParagraphSeparator, '\n');
    const std::string original = selected.toStdString();
    const auto result = transform.apply(original);

    cursor.beginEditBlock();
    for (std::size_t i = 0; i < result.size(); ++i)
    {
        if (original[i] != result[i])
        {
            cursor.setPosition(start + static_cast<int>(i));
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
            cursor.insertText(QString(QChar(result[i])), cursor.charFormat());
        }
    }
    cursor.endEditBlock();
}

void main_window::setup_format_toolbar()
{
    auto* toolbar = addToolBar("Format");
    toolbar->setIconSize(QSize(16, 16));

    auto* action_bold = toolbar->addAction(QIcon("data/images/bold.svg"), "Bold");
    action_bold->setCheckable(true);
    action_bold->setShortcut(QKeySequence("Ctrl+B"));
    connect(action_bold, &QAction::triggered, this, [this](const bool checked)
    {
        QTextCharFormat fmt;
        fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
        editor->mergeCurrentCharFormat(fmt);
    });

    auto* action_italic = toolbar->addAction(QIcon("data/images/italic.svg"), "Italic");
    action_italic->setCheckable(true);
    action_italic->setShortcut(QKeySequence("Ctrl+I"));
    connect(action_italic, &QAction::triggered, this, [this](const bool checked)
    {
        QTextCharFormat fmt;
        fmt.setFontItalic(checked);
        editor->mergeCurrentCharFormat(fmt);
    });

    auto* action_underline = toolbar->addAction(QIcon("data/images/underline.svg"), "Underline");
    action_underline->setCheckable(true);
    action_underline->setShortcut(QKeySequence("Ctrl+U"));
    connect(action_underline, &QAction::triggered, this, [this](const bool checked)
    {
        QTextCharFormat fmt;
        fmt.setFontUnderline(checked);
        editor->mergeCurrentCharFormat(fmt);
    });

    connect(editor, &QTextEdit::currentCharFormatChanged,
            this, [action_bold, action_italic, action_underline](const QTextCharFormat& fmt)
            {
                action_bold->setChecked(fmt.fontWeight() == QFont::Bold);
                action_italic->setChecked(fmt.fontItalic());
                action_underline->setChecked(fmt.fontUnderline());
            });
}
