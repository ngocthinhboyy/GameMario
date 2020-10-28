#include "Brick.h"

Brick::Brick()
{
}

Brick::Brick(float w, float h)
{
	this->w = w;
	this->h = h;
}

void Brick::Render()
{
	RenderBoundingBox();
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + w;
	b = y + h;
}