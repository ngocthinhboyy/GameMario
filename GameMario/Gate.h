#pragma once
#include "GameObject.h"
class Gate : public GameObject
{
	int wayDirectionY;
	float cam_x;
	float cam_y;
	float newPostitionXMario;
	float newPostitionYMario;
	int type;
	bool isUsingThisGate = false;
public:
	Gate(float x, float y, float w, float h, int type, float cam_x, float cam_y, float newPositionXMario, float newPositionYMario, int wayDirectionY);
	void Render();
	void Update(DWORD dt, int scaleTime);
	int GetWayDirectionY() { return wayDirectionY; };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int GetCollisionDirectionY() { return this->wayDirectionY; };
	void RenderBoundingBox();

	bool isOverlapWithMario();

	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
};

