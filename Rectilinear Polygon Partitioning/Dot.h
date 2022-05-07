#pragma once

#include "framework.h"

class Dot {
private:
	RECT rect;
	unsigned int gridX;
	unsigned int gridY;

	void DrawArrow(HDC& hdc, const RECT& _rect, const char direction);	// direction 0: left, 1: up, 2: right, 3: down

public:
	Dot* forward;
	Dot* backward;
	bool selected;

	Dot();
	Dot(const RECT& _rect, const unsigned int _gridX, const unsigned int _gridY);
	Dot(const Dot& other);

	void OnDraw(HDC& hdc);

	const RECT getRect();

	const unsigned int getGridX();

	const unsigned int getGridY();
};