#include "Point.h"
#include "AnimationDatabase.h"
#include "debug.h"

Point::Point(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->vx = 0;
	this->vy = -0.5f;
	this->gameObjectID = idGenerate++;
	timeAppearance = GetTickCount64();
	this->noCollisionConsideration = true;
}

void Point::Update(DWORD dt, int scaleTime)
{
	GameObject::Update(dt, scaleTime);

	if (GetTickCount64() - timeAppearance >= 250) {
		this->stillAlive = false;
	}
	else {
		x += dx;
		y += dy;
	}
}

void Point::Render()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	D3DXVECTOR2 scale = D3DXVECTOR2(3, 3);
	LPANIMATION animation = animationDatabase->Get(2501);
	animation->Render(x, y, 255, scale);
}

void Point::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 39;
	bottom = top + 30;
}
