#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"
class Fireball : public GameObject
{
	LPANIMATION animation;
public:

	Fireball();
	Fireball(float x, float y, float w, float h);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, CollisionMapObject* collisionMapObject);
	void RenderBoundingBox();
};

