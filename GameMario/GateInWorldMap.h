#pragma once
#include "Animation.h"
#include "GameObject.h"
class GateInWorldMap : public GameObject
{
	LPANIMATION animation;
	int type;
	bool isInvisibleGate = false;
	int directionX = 0;
	int directionY = 0;
public:

	GateInWorldMap();
	GateInWorldMap(float x, float y, int w, int h, int type, bool isInvisibleGate, int directionX, int directionY);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int CheckMarioIsStandingOn();

	int GetDirectionX() { return this->directionX; };
	int GetDirectionY() { return this->directionY; };

	void RenderBoundingBox();
};

