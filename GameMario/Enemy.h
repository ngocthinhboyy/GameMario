#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"

class Enemy : public GameObject
{
protected:
	LPANIMATION animation;
	bool isUpsideDown = false;
public:
	virtual void SetAnimation() = 0;
	virtual void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent) = 0;
	virtual void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject) = 0;
	void SetIsUpsideDown(bool isUpsideDown) { this->isUpsideDown = isUpsideDown; };
	bool GetIsUpsideDown() { return this->isUpsideDown; };
	~Enemy();
};

typedef Enemy* LPENEMY;
