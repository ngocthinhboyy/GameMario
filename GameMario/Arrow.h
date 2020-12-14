#pragma once
#include "GameObject.h"
class Arrow : public GameObject
{
	LPANIMATION animation;
public:
	Arrow();
	Arrow(float x, float y, int w, int h);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

