#pragma once
#include "GameObject.h"
class MovingBrick : public GameObject
{
	LPANIMATION animation;
	bool isStanded = false;
public:
	MovingBrick();
	MovingBrick(float x, float y, float w, float h);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

