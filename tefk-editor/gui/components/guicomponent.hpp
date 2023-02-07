#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>

#include "../canvas.hpp"
#include "../color.hpp"
#include "../consoleapi.hpp"
#include "../input.hpp"
#include "../types.h"
#include "window.hpp"

namespace tefk {

enum class SizeBehaviour {
	FIXED,
	CONTENT,
	FILL
};

class Window;

class GUIComponent {
protected:
	Window* _parent;

	Coord _pos;
	Coord _size;
	TextColor _color;
	SizeBehaviour _sizeBehaviour;
	bool _visible;
public:
	GUIComponent();

	void SetParent(Window& window);
	void SetPosition(Coord pos);
	void SetWidth(short width);
	void SetHeight(short height);
	void SetSizeBehaviour(SizeBehaviour sizeBehaviour);
	void SetColor(TextColor color);
	void SetVisible(bool visible);

	Window& GetParent();
	short GetWidth();
	short GetHeight();
	SizeBehaviour GetHeightBehaviour();
	bool IsVisible();
	bool Focused();

	virtual TefkString GetContent() = 0;
	virtual void CatchEvent(Event event) {}
	virtual void DrawOnCanvas() = 0;
};

} // namespace tefk