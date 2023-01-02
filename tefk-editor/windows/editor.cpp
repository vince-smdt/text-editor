#include "editor.hpp"

namespace tefk {

void Editor::Update() {
	_lblHeader.SetText(
		_currFile->GetFilename().generic_string() + " "   // TODO - Find cleaner way of printing info without having
		+ std::to_string(FileIndex()) + "/"               // to call std::to_string() everytime
		+ std::to_string(_files.size()) + " "
		+ "Press Ctrl+S to save!"
	);

	_ediEditor.SetHeight((short)(ConsoleAPI::RowCount() - 2));

	_panFooter.SetPosition({ 0, (short)(ConsoleAPI::RowCount() - 1) });

	_lblFooter.SetPosition({ 0, (short)(ConsoleAPI::RowCount() - 1) });
}

void Editor::CatchEvent(Event& event) {
	if (event.type == Event::Type::KEYPRESS) {
		switch (event.input) {
		case VK_CTRL_S:
			_currFile->SetContent(_ediEditor.GetText());
			_currFile->Save();
			break;
		}
	}
	else if (event.type == Event::Type::CONSOLE_SIZE_CHANGE) {
		_lblFooter.SetText(
			"Rows = " + std::to_string(ConsoleAPI::RowCount())
			+ ", Cols = " + std::to_string(ConsoleAPI::ColCount())
		);
	}
}

void Editor::NewFile() { // TODO - rename functions or fuse newfile with openorcreatefiles
	std::filesystem::path filepath("new.txt");
	_files.push_back(File(filepath)); // TODO - prompt user to enter filename
	_currFile = _files.end() - 1;
}

void Editor::OpenOrCreateFiles(int filecount, char** filenames) {
	for (int i = 1; i < filecount; i++) {
		std::filesystem::path filepath(filenames[i]);
		_files.push_back(File(filepath));
	}
	_currFile = _files.end() - 1;
}

int Editor::FileIndex() {
	return (int)((_currFile - _files.begin()) + 1);
}

}