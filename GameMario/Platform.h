#pragma once
#include "GameObject.h"
class Platform : public GameObject
{
	LPANIMATION animation = NULL;
	DWORD timeStart = 0;
	bool stopMoving = false;
public:
	int type = 0;
	Platform(float x, float y, float w, float h, int type);
	void Update(DWORD dt, int scaleTime);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	~Platform();
};

