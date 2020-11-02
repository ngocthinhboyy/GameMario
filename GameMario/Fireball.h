#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"
#include "Enemy.h"
class Fireball : public GameObject
{
	LPANIMATION animation;
public:

	Fireball();
	Fireball(float x, float y, float w, float h);
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	//void CalcPotentialCollisionsWithEnemy(vector<LPENEMY>* coEnemies, vector<LPCOLLISIONEVENT>& coEvents);

	void CollisionWithOneCollisionMapObject(LPCOLLISIONEVENT collisionEvent, CollisionMapObject* collisionMapObject);
};

