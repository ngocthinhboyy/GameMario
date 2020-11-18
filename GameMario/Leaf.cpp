#include "Leaf.h"
#include "ItemDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"

Leaf::Leaf(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->gameObjectID = idGenerate++;
	this->startTime = GetTickCount64();
	this->startPositionX = x;
	this->startPositionY = y;
}

void Leaf::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	this->animation = animationDatabase->Get(LEAF_ANI);
}

void Leaf::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void Leaf::Update(DWORD dt)
{
	vy += ITEM_GRAVITY * dt;
	if (vy >= 0) {
		if (y < startPositionY) {
			vy = ITEM_GRAVITY * dt * 2;
		}
		else {
			vy = 0;
			vy = ITEM_GRAVITY * dt * 2;
			//DebugOut(L"AAAA \n");
		}
		float time = GetTickCount64() - startTime;
		x = startPositionX - LEAF_AMPLITUDE * cos(LEAF_SPEED * time);
	}
	GameObject::Update(dt);
	y += dy;
}

void Leaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void Leaf::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
}

Leaf::~Leaf()
{
}
