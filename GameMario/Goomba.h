#pragma once
#include "Enemy.h"
#include "CollisionMapObject.h"
class Goomba : public Enemy
{
public:

	Goomba();
	Goomba(float x, float y, float w, float h);
	void SetAnimation();
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithPlayer() {};
	//void CollisionWithAnotherEnemy(DWORD dt, vector<LPENEMY>* enemies = NULL);
};

