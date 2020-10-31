#include "Fireball.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "DynamicObjectDefine.h"

Fireball::Fireball()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	animation = animationDatabase->Get(FIREBALL_ANI_ROLLING);
}

Fireball::Fireball(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	animation = animationDatabase->Get(FIREBALL_ANI_ROLLING);
}

void Fireball::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	if (nx < 1)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}

	//RenderBoundingBox();
}

void Fireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + w;
	b = y + h;
}
