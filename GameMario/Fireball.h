#pragma once
#include "GameObject.h"
class Fireball : public GameObject
{
	LPANIMATION animation;
	DWORD untouchable_start;
	int untouchable;
public:

	Fireball();
	Fireball(float x, float y, float w, float h);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void RenderBoundingBox();
};

