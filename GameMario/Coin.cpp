#include "Coin.h"
#include "AnimationDatabase.h"
#include "ItemDefine.h"
#include "debug.h"

Coin::Coin(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->startPositionY = y;
	this->gameObjectID = idGenerate++;
}

void Coin::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	this->animation = animationDatabase->Get(COIN_ANI);
}

void Coin::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	//RenderBoundingBox();
}

void Coin::Update(DWORD dt)
{
	if (!isBonused) {
		vy += ITEM_GRAVITY * dt;
		GameObject::Update(dt);
		if (y + dy >= startPositionY) {
			this->isBonused = true;
			this->stillAlive = false;
		}
		else {
			y += dy;
		}
	}
}

void Coin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void Coin::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	this->stillAlive = false;
}

Coin::~Coin()
{
}
