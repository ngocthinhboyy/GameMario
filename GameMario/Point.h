#pragma once
#include "GameObject.h"
class Point : public GameObject
{
	DWORD timeAppearance = 0;
	int typePoint;
public:
	Point(float x, float y, float w, float h, int typePoint);
	void Update(DWORD dt, int scaleTime);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

