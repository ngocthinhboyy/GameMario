#include "Flower.h"
#include "EnemyDefine.h"
#include "AnimationDatabase.h"
#include "Mario.h"

Flower::Flower()
{
}

Flower::Flower(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->startPositionX = x;
	this->startPositionY = y;
}

void Flower::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	switch (this->type)
	{
		case RED_FIRE_FLOWER_TYPE: {
			this->animation = animationDatabase->Get(RED_FIRE_FLOWER_ANI);
			break;
		}
		case GREEN_FIRE_FLOWER_TYPE: {
			this->animation = animationDatabase->Get(GREEN_FIRE_FLOWER_ANI);
			break;
		}
		case GREEN_FLOWER_TYPE: {
			this->animation = animationDatabase->Get(GREEN_FLOWER_ANI);
			break;
		}
	default:
		break;
	}
}

void Flower::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	if (Mario::GetInstance()->x > x)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void Flower::Update(DWORD dt)
{
}

void Flower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + FLOWER_BBOX_WIDTH;
	if (type == RED_FIRE_FLOWER_TYPE) {
		b = t + RED_FIRE_FLOWER_BBOX_HEIGHT;
	}
	else {
		b = t + FLOWER_BBOX_HEIGHT;
	}
}

void Flower::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
{
}

void Flower::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
}
