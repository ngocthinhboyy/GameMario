#pragma once
#include "GameObject.h"
class QuestionBrick : public GameObject
{
	LPANIMATION animation;
	float start_Y_position;
	bool alreadyMoving = false;
	int type;
public:
	bool isEmptyBrick = false;
	QuestionBrick();
	QuestionBrick(float x, float y, float w, float h, int type);
	void CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent);
	void Render();
	void Update(DWORD dt, int scaleTime);
	int GetType() { return this->type; };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

