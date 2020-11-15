#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"

class Enemy : public GameObject
{
protected:
	int state;
	LPANIMATION animation;
	bool isUpsideDown = false;
public:
	Enemy();
	void SetState(int state) { this->state = state; };
	virtual void SetAnimation() = 0;
	virtual void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent) = 0;
	virtual void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject) = 0;
	//virtual void CollisionWithAnotherEnemy(DWORD dt, vector<LPENEMY>* enemies = NULL) = 0;
	void SetIsUpsideDown(bool isUpsideDown) { this->isUpsideDown = isUpsideDown; };
	~Enemy();
};

typedef Enemy* LPENEMY;
