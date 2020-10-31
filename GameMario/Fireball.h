#pragma once
#include "GameObject.h"
class Fireball : public GameObject
{
	LPANIMATION animation;
public:

	Fireball();
	Fireball(float x, float y, float w, float h);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

