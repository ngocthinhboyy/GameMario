#pragma once
#include "Item.h"
class Mushroom : public Item
{
public:
	Mushroom(float x, float y, float w, float h);
	void SetAnimation();
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	~Mushroom();
};

