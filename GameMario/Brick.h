#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  48
#define BRICK_BBOX_HEIGHT 48

class Brick : public GameObject
{
public:
	Brick();
	Brick(float w, float h);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};