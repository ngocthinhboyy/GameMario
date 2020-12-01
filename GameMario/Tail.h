#pragma once
#include "GameObject.h"
class Tail : public GameObject
{
	bool hasEffect = false;
	bool enemyIsBack = false;
	LPANIMATION animation;
public:
	Tail();
	Tail(float x, float y, float w, float h);
	void Render();
	void Update(DWORD dt, int scaleTime);
	void SetHasEffect(bool hasEffect) { this->hasEffect = hasEffect; };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void CheckOverlapBoundingBox(vector<LPGAMEOBJECT> objects);
	void RenderBoundingBox();
};
