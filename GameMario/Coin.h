#pragma once
#include "Item.h"

class Coin : public Item
{
	bool isBonused = false;
	int type = 0;
public:
	Coin(float x, float y, float w, float h, int type);
	void SetAnimation();
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetIsBonused(bool isBonused) { this->isBonused = isBonused; };
	bool GetIsBonused() { return this->isBonused; };
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	~Coin();
};

