#pragma once
#include "GameObject.h"
class QuestionBrick : public GameObject
{
	LPANIMATION animation;
public:
	bool isEmptyBrick = false;
	QuestionBrick();
	QuestionBrick(float x, float y, float w, float h);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

