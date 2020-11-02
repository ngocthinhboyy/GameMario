#pragma once
#include "GameObject.h"
class QuestionBrick : public GameObject
{
	LPANIMATION animation;
public:
	QuestionBrick();
	QuestionBrick(float x, float y, float w, float h);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

