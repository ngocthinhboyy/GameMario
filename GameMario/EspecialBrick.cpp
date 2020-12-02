#include "EspecialBrick.h"
#include "AnimationDatabase.h"
#include "StaticObjectDefine.h"

EspecialBrick::EspecialBrick(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->gameObjectID = idGenerate++;
}

void EspecialBrick::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
}

void EspecialBrick::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	animation = AnimationDatabase::GetInstance()->Get(ESPECIAL_BRICK_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void EspecialBrick::Update(DWORD dt, int scaleTime)
{
}

void EspecialBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
