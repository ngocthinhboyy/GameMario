#pragma once
#include "GameObject.h"
class QuestionBrick : public GameObject
{
	LPANIMATION animation;
	float start_Y_position;
	bool alreadyMoving = false;
public:
	bool isEmptyBrick = false;
	QuestionBrick();
	QuestionBrick(float x, float y, float w, float h);
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

