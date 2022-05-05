#include "Dot.h"

Dot::Dot() :
	rect(RECT()),
	gridX(0),
	gridY(0),
	forward(nullptr),
	backward(nullptr),
	selected(false)
{}
Dot::Dot(const RECT _rect, const unsigned int _gridX, const unsigned int _gridY) :
	rect(_rect),
	gridX(_gridX),
	gridY(_gridY),
	forward(nullptr),
	backward(nullptr),
	selected(false)
{}
Dot::Dot(const Dot& other) :
	rect(other.rect),
	gridX(other.gridX),
	gridY(other.gridY),
	forward(other.forward),
	backward(other.backward),
	selected(other.selected)
{}

VOID Dot::OnDraw(HDC& hdc) {
	SelectObject(hdc, CreateSolidBrush(RGB(selected ? 255 : 0, 0, selected ? 0 : 255)));
	Ellipse(
		hdc,
		rect.left,
		rect.top,
		rect.right,
		rect.bottom
	);
	if (forward != nullptr) {
		RECT forwardRect = forward->getRect();
		bool sameX = gridX == forward->getGridX();
		bool sameY = gridY == forward->getGridY();
		SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
		Rectangle(
			hdc,
			sameX ? rect.left : min(rect.right, forwardRect.right),
			sameY ? rect.top : min(rect.bottom, forwardRect.bottom),
			sameX ? rect.right : max(rect.left, forwardRect.left),
			sameY ? rect.bottom : max(rect.top, forwardRect.top)
		);
	}
}

const RECT Dot::getRect() {
	return rect;
}

const unsigned int Dot::getGridX() {
	return gridX;
}

const unsigned int Dot::getGridY() {
	return gridY;
}