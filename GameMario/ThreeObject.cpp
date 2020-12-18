#include "ThreeObject.h"
#include "WorldMapDefine.h"
#include "AnimationDatabase.h"
#include "IntroMapDefine.h"

ThreeObject::ThreeObject()
{
}

ThreeObject::ThreeObject(float x, float y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void ThreeObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void ThreeObject::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	animation = AnimationDatabase::GetInstance()->Get(THREEOBJECT_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}
