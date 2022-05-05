#include "Grid.h"

Grid::Grid() :
	rect(RECT()),
	dots(std::vector<std::vector<Dot>>())
{}
Grid::Grid(HWND& hwnd, const unsigned int cols, const unsigned int rows) {
	RECT clientRect = RECT();
	GetClientRect(hwnd, &clientRect);
	const unsigned int clientWidth = clientRect.right - clientRect.left;
	const unsigned int clientHeight = clientRect.bottom - clientRect.top;
	const float spacing = min(clientWidth * 0.8f / cols, clientHeight * 0.8f / rows);
	const float thisWidth = spacing * cols;
	const float thisHeight = spacing * rows;
	rect.left = (clientWidth - thisWidth) / 2;
	rect.top = (clientHeight - thisHeight) / 2;
	rect.right = rect.left + thisWidth;
	rect.bottom = rect.top + thisHeight;

	dots = std::vector<std::vector<Dot>>();
	const float dotSize = spacing * 0.2f;
	const float dotOffset = (spacing - dotSize) / 2;
	const float dotLeftOffset = rect.left + dotOffset;
	const float dotTopOffset = rect.top + dotOffset;
	for (unsigned int col = 0; col < cols; col++) {
		std::vector<Dot> dotCol = std::vector<Dot>();
		for (unsigned int row = 0; row < rows; row++) {
			RECT dotRect = RECT();
			dotRect.left = dotLeftOffset + (spacing * col);
			dotRect.top = dotTopOffset + (spacing * row);
			dotRect.right = dotRect.left + dotSize;
			dotRect.bottom = dotRect.top + dotSize;
			dotCol.push_back(Dot(dotRect, col, row));
		}
		dots.push_back(dotCol);
	}
}
Grid::Grid(const Grid& other) :
	rect(other.rect),
	dots(other.dots)
{}

VOID Grid::OnDraw(HDC& hdc) {
	const float cornerRadius = min(rect.right - rect.left, rect.bottom - rect.top) / 10;
	SelectObject(hdc, CreateSolidBrush(RGB(100, 100, 100)));
	RoundRect(
		hdc,
		rect.left,
		rect.top,
		rect.right,
		rect.bottom,
		cornerRadius,
		cornerRadius
	);

	for (std::vector<Dot>& dotCol : dots) {
		for (Dot& dot : dotCol) {
			dot.OnDraw(hdc);
		}
	}
}

const RECT Grid::getRect() {
	return rect;
}