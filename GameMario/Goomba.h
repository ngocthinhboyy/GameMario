#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"
class Goomba : public GameObject
{
public:

	Goomba();
	Goomba(float x, float y, float w, float h);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, CollisionMapObject* collisionMapObject);
};

