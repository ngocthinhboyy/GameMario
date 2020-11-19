#pragma once
#include "Enemy.h"
class Flower : public Enemy
{
	int type = 0;
	bool isMoving = true;
	bool isGoUp = true;
	int startAttacking = 0;
	bool finishedAttacking = false;
	int state = 0;
	int lastMoving = 0;
public:

	Flower();
	Flower(float x, float y, float w, float h, int type);
	void SetAnimation();
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
};
