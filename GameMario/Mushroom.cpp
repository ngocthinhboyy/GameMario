#include "Mushroom.h"

Mushroom::Mushroom(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->gameObjectID = idGenerate++;
}

void Mushroom::SetAnimation()
{
}

void Mushroom::Render()
{
}

void Mushroom::Update(DWORD dt)
{
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void Mushroom::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
}

Mushroom::~Mushroom()
{
}
