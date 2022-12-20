#pragma once
#include <algorithm>
#include <memory>
#include <vector>
#include "color.h"
#include "consoleapi.hpp"

namespace tefk {

class Window;

class GUIComponent : public std::enable_shared_from_this<GUIComponent> {
protected:
	Coord _pos;
	Coord _size;
	TextColor _color;
	std::shared_ptr<Window> _parent;
	
	const Coord _ORIGIN = { 0, 0 };
public:
	GUIComponent()
		: _pos{ _ORIGIN },
		  _size{ _ORIGIN },
		  _color{ BLACK, BLACK } // TODO - set to default component color?
	{}

	Coord GetPosition() { return _pos; }
	Coord GetSize() { return _size; }
	TextColor GetColor() { return _color; }
	std::shared_ptr<Window> GetParent() { return _parent; }

	void SetPosition(Coord pos) { _pos = pos; }
	void SetSize(Coord size) { _size = size; }
	void SetColor(TextColor color) { _color = color; }
	void SetParent(std::shared_ptr<Window> window) { _parent = window; }

	virtual void Print() {}
};

}
