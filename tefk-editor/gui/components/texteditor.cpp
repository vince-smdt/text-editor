#include "texteditor.hpp"

namespace tefk {

TextEditor::TextEditor()
	: GUIComponent{}
{}

TextEditor::TextEditor(Coord pos, Coord size, TextColor color)
	: GUIComponent{ pos, size, color }
{}

void TextEditor::SetText(std::string text) {
	size_t currLineBreak = 0,
	       nextLineBreak = 0;

	_rows.clear();
	while (true) {
		nextLineBreak = text.find_first_of('\n', currLineBreak);
		_rows.push_back(text.substr(currLineBreak, nextLineBreak - currLineBreak));
		if (nextLineBreak == std::string::npos)
			break;
		currLineBreak = ++nextLineBreak;
	};

	// If text empty, push at least one row into _rows to position cursor
	if (_rows.empty())
		_rows.push_back("");

	_cursorRow = _rows.begin();
	_cursorCol = _cursorRow->begin();
}

std::string TextEditor::GetText() {
	std::string text = "";
	for (std::string row : _rows)
		text.append(row + '\n');
	return text.substr(0, text.size() - 1);
}

void TextEditor::CatchEvent(Event& event) {
	if (event.type == Event::Type::CHARACTER) {
		switch (event.input) {
		case VK_BACK:
			DeleteChar();
			break;
		case VK_CTRL_BACKSPACE:
			DeleteWord();
			break;
		case VK_RETURN:
			NewLine();
			break;
		default:
			if (event.input > 26)
				AddChar(event.input);
			break;
		}
	}
	else if (event.type == Event::Type::SPECIAL_CHARACTER) {
		switch (event.input) {
		case VK_ARROW_RIGHT:
			MoveCursorRight();
			break;
		case VK_ARROW_LEFT:
			MoveCursorLeft();
			break;
		case VK_ARROW_UP:
			MoveCursorUp();
			break;
		case VK_ARROW_DOWN:
			MoveCursorDown();
			break;
		}
	}
}

void TextEditor::AddChar(char ch) {
	_cursorCol = _cursorRow->insert(_cursorCol, ch) + 1;
}

void TextEditor::NewLine() {
	// TODO - if newline in middle of row, move content to next line
	_cursorRow = _rows.insert(_cursorRow + 1, "");
	_cursorCol = _cursorRow->begin();
}

void TextEditor::DeleteChar() {
	// If at beginning of row
	if (_cursorCol == _cursorRow->begin()) {
		// If at begining of file, cancel
		if (_cursorRow == _rows.begin())
			return;

		// Move content of current row to previous row, then delete current row
		size_t currRowSize = (_cursorRow - 1)->size();
		(_cursorRow - 1)->append(*_cursorRow);
		_cursorRow = _rows.erase(_cursorRow);
		_cursorCol = (--_cursorRow)->begin() + currRowSize;
		return;
	}
	_cursorRow->erase(--_cursorCol);
}

void TextEditor::DeleteWord() {
	// TODO - this function
}

void TextEditor::MoveCursorRight() {
	// If at end of row
	if (_cursorCol == _cursorRow->end()) {
		// Cancel if cursor already at eof
		if (_cursorRow == _rows.end() - 1)
			return;

		// Move cursor to next row
		_cursorCol = (++_cursorRow)->begin();
		return;
	}

	// Move cursor to next char
	_cursorCol++;
}

void TextEditor::MoveCursorLeft() {
	// If at beginning of row
	if (_cursorCol == _cursorRow->begin()) {
		// Cancel if cursor already at bof
		if (_cursorRow == _rows.begin())
			return;

		// Move cursor to previous row
		_cursorCol = (--_cursorRow)->end();
		return;
	}

	// Move cursor to previous char
	_cursorCol--;
}

void TextEditor::MoveCursorUp() {
	// If at first row
	if (_cursorRow == _rows.begin()) {
		// Move cursor to beginning of line
		_cursorCol = _cursorRow->begin();
		return;
	}

	// Move cursor to previous row
	int colIndex = _cursorCol - _cursorRow->begin();
	_cursorRow--;
	
	// If cursor column index is bigger than new row, move to end of new row
	// Else, move to same column index of new row
	if (colIndex >= _cursorRow->size()) {
		_cursorCol = _cursorRow->end();
		return;
	}
	_cursorCol = _cursorRow->begin() + colIndex;

}

void TextEditor::MoveCursorDown() {
	// If at last row
	if (_cursorRow == _rows.end() - 1) {
		// Move cursor to end of line
		_cursorCol = _cursorRow->end();
		return;
	}

	// Move cursor to next row
	int colIndex = _cursorCol - _cursorRow->begin();
	_cursorRow++;

	// If cursor column index is bigger than new row, move to end of new row
	// Else, move to same column index of new row
	if (colIndex >= _cursorRow->size()) {
		_cursorCol = _cursorRow->end();
		return;
	}
	_cursorCol = _cursorRow->begin() + colIndex;
}

void TextEditor::PrintContent() {
	// Print text
	for (size_t currRow = 0; currRow < _size.Y && currRow + _pos.Y < ConsoleAPI::RowCount(); currRow++) {
		ConsoleAPI::SetCursorPos(_pos.Y + currRow, _pos.X);

		std::string rowText = (currRow < _rows.size())
			? _rows[currRow]
			: "";
		
		std::string row = (rowText.size() <= _size.X)
			? rowText + std::string(_size.X - rowText.size(), ' ')
			: rowText.substr(0, _size.X);
		
		std::cout.write(row.c_str(), row.size());
	}

	PrintCursor();
}

void TextEditor::PrintCursor() {
	Coord cursorPos = {
		_pos.X + _cursorCol - _cursorRow->begin(),
		_pos.Y + _cursorRow - _rows.begin()
	};

	// Cancel if cursor out of print area
	// TODO - hide cursor if not in texteditor, not just out of console window
	if (cursorPos.X >= ConsoleAPI::ColCount() || cursorPos.X < 0 || cursorPos.Y >= ConsoleAPI::RowCount() || cursorPos.Y < 0)
		return;

	ConsoleAPI::SetTextColor(_color.Inverse());
	ConsoleAPI::SetCursorPos(cursorPos.Y, cursorPos.X);
	if (_cursorCol != _cursorRow->end())
		std::cout.write(std::string(1, *_cursorCol).c_str(), 1);
	else
		std::cout.write(" ", 1);
}

}