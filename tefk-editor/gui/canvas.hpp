#pragma once
#include <vector>

#include "color.hpp"
#include "consoleapi.hpp"

namespace tefk {

struct Pixel {
	TefkChar character;
	TextColor color;
};

class Canvas {
	std::vector<Pixel> _pixels;
	Coord _dimensions;
	
	Canvas();
public:
	static Canvas& Instance();

	void Render();
	Pixel& PixelAt(size_t x, size_t y);
	void Resize(size_t x, size_t y);
};

Canvas& GetCanvas();

} // namespace tefk