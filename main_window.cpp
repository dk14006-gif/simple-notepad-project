#include "main_window.h"

#include <QTextEdit>
#include <QMenuBar>

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
}

main_window::~main_window() = default;

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
