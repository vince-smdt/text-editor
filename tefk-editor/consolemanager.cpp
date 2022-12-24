#include "consolemanager.hpp"

namespace tefk {

int ConsoleManager::s_currRows = 0;
int ConsoleManager::s_currCols = 0;
std::shared_ptr<Window> ConsoleManager::s_window;
TextColor ConsoleManager::_defaultColor = { BLACK, WHITE };
TextColor ConsoleManager::_headerColor = { WHITE, BLACK };
TextColor ConsoleManager::_contentColor = { BLACK, WHITE };
TextColor ConsoleManager::_footerColor = { WHITE, BLACK };

void ConsoleManager::Init() {
	s_window = Designer::Editor();
}

bool ConsoleManager::ConsoleSizeChanged() {
	return s_currRows != ConsoleAPI::RowCount() || s_currCols != ConsoleAPI::ColCount();
}

void ConsoleManager::RefreshConsole() {
	// TODO - Reset entire console color in case previous color before app starts
	s_currRows = ConsoleAPI::RowCount();
	s_currCols = ConsoleAPI::ColCount();

	ConsoleAPI::ClearConsole();
	ConsoleAPI::SetConsoleSize(s_currRows, s_currCols);

	s_window->Print();
}

}