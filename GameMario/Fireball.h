#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"
#include "Enemy.h"
class Fireball : public GameObject
{
	LPANIMATION animation;
	int type = 0;
	bool isDie = false;
public:

	Fireball();
	Fireball(float x, float y, float w, float h, int type);
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetAnimation();
	//void CalcPotentialCollisionsWithEnemy(vector<LPENEMY>* coEnemies, vector<LPCOLLISIONEVENT>& coEvents);
	bool IsOverlapBoundingBoxWithMario();
	bool IsOverlapWithEnemy(vector<LPGAMEOBJECT> enemies);

	void CollisionWithOneCollisionMapObject(LPCOLLISIONEVENT collisionEvent, CollisionMapObject* collisionMapObject);
};

