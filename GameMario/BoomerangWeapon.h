#pragma once
#include "Enemy.h"
class BoomerangWeapon : public Enemy
{
	DWORD timeTurnBack = 0;
	bool isTurningBack = false;
	bool isAlreadyTurnedBack = false;
public:

	BoomerangWeapon();
	BoomerangWeapon(float x, float y, float w, float h);
	void SetAnimation();
	void Render();
	void ChangeState();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
};

