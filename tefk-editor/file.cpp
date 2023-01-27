#include "file.hpp"

namespace tefk {

File::File() {}

std::filesystem::path File::GetFilename() {
	return _filename;
}

std::string File::GetContent() {
	return _content;
}

void File::SetContent(std::string content) {
	_content = content;
}

bool File::Open(std::filesystem::path& filename) {
	// If file doesn't exist
	if (!std::filesystem::exists(filename)) {
		_filename = filename;
		return true;
	}

	std::ifstream file(filename);

	// If we have access to file
	if (file) {
		// TODO - (maybe) optimize to avoid redundant copy https://stackoverflow.com/questions/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c
		std::ostringstream ss;
		ss << file.rdbuf();
		_filename = filename;
		_content = ss.str();
		return true;
	}

	// If we don't have access to file
	return false;
}

void File::Save() {
	std::ofstream file(_filename);
	file << _content;
}

} // namespace tefk