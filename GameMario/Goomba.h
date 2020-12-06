#pragma once
#include "Enemy.h"
#include "CollisionMapObject.h"
class Goomba : public Enemy
{
	int type;
	DWORD timeWalkingType2 = 0;
	DWORD timeJumpType2 = 0;
	int countTimeJump = 0;
	DWORD timeDie = 0;
public:

	Goomba();
	Goomba(float x, float y, float w, float h, int type);
	void SetAnimation();
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	void SetStartPosition();
	void ChangeStateRedGoomba();
	int GetType() { return this->type; };
	//void CollisionWithAnotherEnemy(DWORD dt, vector<LPENEMY>* enemies = NULL);
};

