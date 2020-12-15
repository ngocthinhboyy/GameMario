#pragma once
#include "Item.h"
#include "CollisionMapObject.h"
class Mushroom : public Item
{
	bool alreadyAppear = false;
	int typeMushroom = 1;
public:
	Mushroom(float x, float y, float w, float h, int typeMushroom);
	void SetAnimation();
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	~Mushroom();
};

