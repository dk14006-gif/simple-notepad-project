# Notepad

## Overview

This project extends the base Notepad application into a more capable WordPad-like editor
with rich text formatting, spell checking, and several optional features.

## Required Features

### 1. Exception Handling

The file `notepad_exception.h` defines a hierarchy of four exception classes:
- `notepad_exception` — base class, inherits from `std::runtime_error`
- `file_not_found_exception` — thrown when a file path does not exist
- `file_read_exception` — thrown when a file cannot be opened for reading
- `file_write_exception` — thrown when a file cannot be opened for writing

Both `open_file()` and `save_file()` in `main_window.cpp` are wrapped in `try/catch`
blocks that catch `notepad_exception` and display the error via `QMessageBox::critical`
with title `"Error"`.

### 2. Spell Checker

Split into two classes:

- `spell_checker` — loads `data/words.txt` into a `std::set<std::string>` at startup.
  `is_correct()` normalizes a word (lowercase, strip non-alpha) and looks it up in the set.
  `suggestions()` finds words with the same prefix using `lower_bound`.

- `spell_checker_highlighter` — a `QSyntaxHighlighter` subclass that underlines
  misspelled words with `QTextCharFormat::SpellCheckUnderline` in red.
  Runs in real time as the user types.

Right-clicking a misspelled word shows a context menu with up to 5 suggestions.
**Tools → Check Spelling...** re-runs `rehighlight()` over the entire document.

## Optional Features

### 1. Cursor Line / Column Indicator

A permanent `QLabel` in the status bar displays `"Ln X, Col Y"` and is updated
on every `cursorPositionChanged` signal. Also shows word count and line count.

### 2. Font Dialog

**Format → Font...** opens `QFontDialog::getFont()`. If text is selected,
the font is applied to the selection. Otherwise it changes the whole document font.

### 3. Color Picker

**Format → Text Color...** opens `QColorDialog::getColor()`.
The chosen color is applied to the current selection via `QTextCharFormat::setForeground()`.

### 4. Zoom

**View → Zoom In** (`Ctrl++`), **Zoom Out** (`Ctrl+-`), **Reset Zoom** (`Ctrl+0`)
use `QTextEdit::zoomIn()` / `zoomOut()` in steps of 2 points.
`m_zoom_level` tracks the total offset so Reset can exactly undo all changes.

### 5. Line Numbers

A custom `line_number_area` widget draws line numbers in a grey margin
beside the editor. It repaints on scroll, cursor movement, and block count changes.

## Build Instructions

```bash
cmake -S . -B build
cmake --build build
```

```powershell
cmake -S . -B build -G "Ninja" -DCMAKE_PREFIX_PATH="C:/Qt/6.10.2/mingw_64"
cmake --build build
```

Set working directory to project root so that `data/` resolves correctly.