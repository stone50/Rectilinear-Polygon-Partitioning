#include "Dot.h"

void Dot::DrawArrow(HDC& hdc,  const RECT& _rect, const char direction) {
	POINT triangle[3] = {
		POINT(),
		POINT(),
		POINT()
	};
	RECT stem = RECT();

	switch (direction) {
	// left
	case 0:
		{
			const int width = _rect.bottom - _rect.top;
			const int leftOffset = _rect.left + width;
			triangle[0] = POINT{
				_rect.left,
				_rect.top + (width / 2)
			};
			triangle[1] = POINT{
				leftOffset,
				_rect.top
			};
			triangle[2] = POINT{
				leftOffset,
				_rect.bottom
			};
			const int halfStemWidth = width / 4;
			stem.left = leftOffset;
			stem.top = _rect.top + halfStemWidth;
			stem.right = _rect.right;
			stem.bottom = _rect.bottom - halfStemWidth;
		}
		break;

	// up
	case 1:
		{
			const int width = _rect.right - _rect.left;
			const int topOffset = _rect.top + width;
			triangle[0] = POINT{
				_rect.left + (width / 2),
				_rect.top
			};
			triangle[1] = POINT{
				_rect.right,
				topOffset
			};
			triangle[2] = POINT{
				_rect.left,
				topOffset
			};
			const int halfStemWidth = width / 4;
			stem.left = _rect.left + halfStemWidth;
			stem.top = topOffset;
			stem.right = _rect.right - halfStemWidth;
			stem.bottom = _rect.bottom;
		}
		break;

	// right
	case 2:
		{
			const int width = _rect.bottom - _rect.top;
			const int rightOffset = _rect.right - width;
			triangle[0] = POINT{
				_rect.right,
				_rect.top + (width / 2)
			};
			triangle[1] = POINT{
				rightOffset,
				_rect.bottom
			};
			triangle[2] = POINT{
				rightOffset,
				_rect.top
			};
			const int halfStemWidth = width / 4;
			stem.left = _rect.left;
			stem.top = _rect.top + halfStemWidth;
			stem.right = rightOffset;
			stem.bottom = _rect.bottom - halfStemWidth;
		}
		break;

	// down
	case 3:
		{
			const int width = _rect.right - _rect.left;
			const int bottomOffset = _rect.bottom - width;
			triangle[0] = POINT{
				_rect.left + (width / 2),
				_rect.bottom
			};
			triangle[1] = POINT{
				_rect.left,
				bottomOffset
			};
			triangle[2] = POINT{
				_rect.right,
				bottomOffset
			};
			const int halfStemWidth = width / 4;
			stem.left = _rect.left + halfStemWidth;
			stem.top = _rect.top;
			stem.right = _rect.right - halfStemWidth;
			stem.bottom = bottomOffset;
		}
		break;
	}

	Polygon(
		hdc,
		triangle,
		3
	);
	Rectangle(
		hdc,
		stem.left,
		stem.top,
		stem.right,
		stem.bottom
	);
}

Dot::Dot() :
	rect(RECT()),
	gridX(0),
	gridY(0),
	forward(nullptr),
	backward(nullptr),
	selected(false)
{}
Dot::Dot(const RECT& _rect, const unsigned int _gridX, const unsigned int _gridY) :
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

void Dot::OnDraw(HDC& hdc) {
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
		RECT arrowRect {
			sameX ? rect.left : min(rect.right, forwardRect.right),
			sameY ? rect.top : min(rect.bottom, forwardRect.bottom),
			sameX ? rect.right : max(rect.left, forwardRect.left),
			sameY ? rect.bottom : max(rect.top, forwardRect.top)
		};
		DrawArrow(
			hdc,
			arrowRect,
			sameX ? (forward->getGridY() > gridY ? 3 : 1) : (forward->getGridX() > gridX ? 2 : 0)
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