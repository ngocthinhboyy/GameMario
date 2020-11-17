#pragma once
#include "GameObject.h"

class Item : public GameObject
{
protected:
	int state;
	LPANIMATION animation;
public:
	Item();
	void SetState(int state) { this->state = state; };
	virtual void SetAnimation() = 0;
	virtual void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent) = 0;
	~Item();
};
typedef Item* LPITEM;

