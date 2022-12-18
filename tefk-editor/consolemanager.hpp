#pragma once
#include <iostream>
#include <memory>
#include "editor.hpp"
#include "gui-components/panel.hpp"
#include "gui-components/label.hpp"

namespace tefk {


class ConsoleManager {
private:
	ConsoleManager() = delete;
	ConsoleManager(const ConsoleManager&) = delete;

	static int s_currRows;
	static int s_currCols;

	static std::shared_ptr<Panel> s_header;

	// TODO - make these constants
	static TextColor _defaultColor;
	static TextColor _headerColor;
	static TextColor _contentColor;
	static TextColor _footerColor;
public:
	static void Init() {
		s_header = std::make_shared<Panel>();
		s_header->SetSize({ 26, 4 });
		s_header->SetColor({ BLUE, WHITE });
		s_header->SetPosition({ 40, 20 });
		
		std::shared_ptr<Label> t = std::make_shared<Label>();
		t->SetSize({ 26, 4 });
		t->SetColor({ BLUE, WHITE });
		t->SetPosition({ 0, 0 });
		t->SetText("This is a test to test labels and if their text wraps correctly in a container too small to fully display them.");

		s_header->AddComponent(t);
	}

	static bool ConsoleSizeChanged() {
		return s_currRows != ConsoleAPI::RowCount() || s_currCols != ConsoleAPI::ColCount();
	}

	static void Print(std::string text, TextColor color) {
		ConsoleAPI::SetTextColor(color);

		if (text.size() > ConsoleAPI::ColCount())
			text = text.substr(0, ConsoleAPI::ColCount() - 1);
		std::cout << text << FillRow;

		ConsoleAPI::SetTextColor(_defaultColor);
	}

	static void PrintHeader() {
		ConsoleAPI::SetCursorPos(0, 0);

		std::stringstream ss;
		ss << Editor::CurrentFile().GetFilename() << " "
		   << Editor::FileIndex() + 1 << "/"
		   << Editor::Files().size() << " "
		   << "Press Ctrl+S to save!";
		Print(ss.str(), _headerColor);
	}

	static void PrintContent() {
		// TODO - Only print content that fits in the console, rest can be seen by scrolling
		ConsoleAPI::SetCursorPos(2, 0);

		std::stringstream ss;
		ss << Editor::CurrentFile().GetContent();
		Print(ss.str(), _contentColor);
	}

	static void PrintFooter() {
		ConsoleAPI::SetCursorPos(ConsoleAPI::RowCount() - 1, 0);

		std::stringstream ss;
		ss << "Rows = " << ConsoleAPI::RowCount() 
		   << ", Cols = " << ConsoleAPI::ColCount();
		Print(ss.str(), _footerColor);
	}

	static std::ostream& FillRow(std::ostream& stream) {
		return stream << std::string(ConsoleAPI::ColCount() - ConsoleAPI::CursorColPos(), ' ');
	}

	static void RefreshConsole() {
		// TODO - Reset entire console color in case previous color before app starts
		s_currRows = ConsoleAPI::RowCount();
		s_currCols = ConsoleAPI::ColCount();

		// TODO - Maybe resize console after clearing it to avoid console resize error
		ConsoleAPI::SetConsoleSize(s_currRows, s_currCols);

		ConsoleAPI::ClearConsole();

		PrintHeader();
		PrintContent();
		PrintFooter();

		s_header->Print();
	}
};
int ConsoleManager::s_currRows = 0;
int ConsoleManager::s_currCols = 0;
std::shared_ptr<Panel> ConsoleManager::s_header;
TextColor ConsoleManager::_defaultColor = { BLACK, WHITE };
TextColor ConsoleManager::_headerColor = { WHITE, BLACK };
TextColor ConsoleManager::_contentColor = { BLACK, WHITE };
TextColor ConsoleManager::_footerColor = { WHITE, BLACK };

}