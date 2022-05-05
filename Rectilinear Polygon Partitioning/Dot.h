#pragma once

#include "framework.h"

class Dot {
private:
	RECT rect;
	unsigned int gridX;
	unsigned int gridY;

public:
	Dot* forward;
	Dot* backward;
	bool selected;

	Dot();
	Dot(const RECT _rect, const unsigned int _gridX, const unsigned int _gridY);
	Dot(const Dot& other);

	VOID OnDraw(HDC& hdc);

	const RECT getRect();

	const unsigned int getGridX();

	const unsigned int getGridY();
};