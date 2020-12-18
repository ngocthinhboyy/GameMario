#include "Arrow.h"
#include "AnimationDatabase.h"
#include "IntroMapDefine.h"

Arrow::Arrow()
{
}

Arrow::Arrow(float x, float y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->startPositionX = x;
	this->startPositionY = y;
}

void Arrow::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	animation = AnimationDatabase::GetInstance()->Get(ARROW_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void Arrow::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
