#pragma once

#include <vector>
#include "Dot.h"

class Grid {
private:
	RECT rect;

public:
	std::vector<std::vector<Dot>> dots;
	
	Grid();
	Grid(HWND& hwnd, const unsigned int cols, const unsigned int rows);
	Grid(const Grid& other);

	void OnDraw(HDC& hdc);

	const RECT getRect();

};