#pragma once
#include "Enemy.h"
class BoomerangBrother : public Enemy
{
	DWORD timeMove = 0;
	DWORD timeAttack = 0;
public:

	BoomerangBrother();
	BoomerangBrother(float x, float y, float w, float h);
	void SetAnimation();
	void Render();
	void ChangeState();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
};

