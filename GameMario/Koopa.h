#pragma once
#include "Enemy.h"
class Koopa : public Enemy
{
	int type;
	bool isHold = false;
public:

	Koopa();
	Koopa(float x, float y, float w, float h, int type);
	void Render();
	void SetAnimation();
	void Update(DWORD dt);
	void SetIsHold(bool isHold) { this->isHold = isHold; };
	bool GetIsHold() { return isHold; };
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	//void CollisionWithAnotherEnemy(DWORD dt, vector<LPENEMY>* enemies = NULL);
};

