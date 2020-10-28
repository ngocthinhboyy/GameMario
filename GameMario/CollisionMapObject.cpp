#include "CollisionMapObject.h"

CollisionMapObject::CollisionMapObject()
{
}

CollisionMapObject::CollisionMapObject(float w, float h, int colDirectX, int colDirectY)
{
	this->w = w;
	this->h = h;
	this->collisionDirectionX = colDirectX;
	this->collisionDirectionY = colDirectY;
}

void CollisionMapObject::Render()
{
	RenderBoundingBox();
}

void CollisionMapObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + w;
	b = y + h;
}
