#pragma once
#include "GameObject.h"
class Tail : public GameObject
{
public:
	Tail();
	Tail(float x, float y, float w, float h);
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void CheckOverlapBoundingBox(vector<LPGAMEOBJECT> enemies);
	void RenderBoundingBox();
};
