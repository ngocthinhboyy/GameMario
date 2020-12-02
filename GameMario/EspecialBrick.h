#pragma once
#include "GameObject.h"
class EspecialBrick : public GameObject
{
	LPANIMATION animation;
	int type;
public:
	EspecialBrick(float x, float y, float w, float h, int type);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	void Render();
	void Update(DWORD dt, int scaleTime);
	int GetType() { return this->type; };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

