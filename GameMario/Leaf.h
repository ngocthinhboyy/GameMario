#pragma once
#include "Item.h"
class Leaf : public Item
{
	int startTime;
	bool isSupplyFirstSpeed = true;
	bool isGoUp = false;
	bool firstTimeAppear = false;
public:
	Leaf(float x, float y, float w, float h);
	void SetAnimation();
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	~Leaf();
};

