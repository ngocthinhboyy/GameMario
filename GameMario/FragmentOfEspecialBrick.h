#pragma once
#include "GameObject.h"
class FragmentOfEspecialBrick : public GameObject
{
	LPANIMATION animation = NULL;
	int type = 0;
	DWORD timeStart = 0;
public:
	FragmentOfEspecialBrick(float x, float y, float w, float h, float vx, float vy);
	void Render();
	void Update(DWORD dt, int scaleTime);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

