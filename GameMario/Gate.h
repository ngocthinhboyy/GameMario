#pragma once
#include "GameObject.h"
class Gate : public GameObject
{
	int wayDirectionY;
	float cam_x;
	float cam_y;
	float newPostitionMario;
	int type;
public:
	Gate(float x, float y, float w, float h, int type, float cam_x, float cam_y, float newPositionMario, int wayDirectY);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int GetCollisionDirectionY() { return this->wayDirectionY; };
	void RenderBoundingBox();
};

