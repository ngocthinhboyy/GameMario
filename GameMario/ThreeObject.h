#pragma once
#include "GameObject.h"
class ThreeObject : public GameObject
{
	LPANIMATION animation;
public:
	ThreeObject();
	ThreeObject(float x, float y, int w, int h);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
};

