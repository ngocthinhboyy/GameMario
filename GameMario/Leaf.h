#pragma once
#include "Item.h"
class Leaf : public Item
{
	int startTime;
	bool isCapVDau = true;
	bool isGoUp = false;
	bool vuamoirakhoi = false;
public:
	Leaf(float x, float y, float w, float h);
	void SetAnimation();
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	~Leaf();
};

