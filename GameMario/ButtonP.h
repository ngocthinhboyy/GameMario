#pragma once
#include "GameObject.h"
class ButtonP : public GameObject
{
	LPANIMATION animation;
	bool alreadyPress = false;
public:
	ButtonP(float x, float y, float w, float h);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

