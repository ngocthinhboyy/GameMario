#pragma once
#include "GameObject.h"
class RandomGift : public GameObject
{
	LPANIMATION animation;
	int itemCurrent = 0;
	DWORD timeStartRandom = 0;
public:
	RandomGift() {};
	RandomGift(float x, float y, float w, float h);
	void SetAnimation();
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
};

