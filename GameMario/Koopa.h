#pragma once
#include "Enemy.h"
class Koopa : public Enemy
{
	int type;
	bool isHold = false;
	bool isDiedByFireball = false;
	int startTypeMove;
	DWORD timeDie = 0;
public:

	Koopa();
	Koopa(float x, float y, float w, float h, int typeKoopa, int typeMove);
	void Render();
	void SetAnimation();
	void SetIsDiedByFireball() { this->isDiedByFireball = true; };
	void SetTimeDie() { this->timeDie = GetTickCount64(); };
	void Update(DWORD dt);
	void SetIsHold(bool isHold) { this->isHold = isHold; };
	bool GetIsHold() { return isHold; };
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	void SetStartPosition();
	//void CollisionWithAnotherEnemy(DWORD dt, vector<LPENEMY>* enemies = NULL);
};

