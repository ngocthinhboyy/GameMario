#include "Point.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "StaticObjectDefine.h"

Point::Point(float x, float y, float w, float h, int typePoint)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->vx = 0;
	this->vy = -0.5f;
	this->typePoint = typePoint;
	this->gameObjectID = idGenerate++;
	timeAppearance = GetTickCount64();
	this->noCollisionConsideration = true;
}

void Point::Update(DWORD dt, int scaleTime)
{
	GameObject::Update(dt, 1);

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
	D3DXVECTOR2 scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	LPANIMATION animation = NULL;
	if(typePoint == POINT_TYPE_100)
		animation = animationDatabase->Get(POINT_100_ANI);
	else if (typePoint == POINT_TYPE_1000) {
		animation = animationDatabase->Get(POINT_1000_ANI);
	}
	else if (typePoint == POINT_TYPE_1UP) {
		animation = animationDatabase->Get(POINT_1UP_ANI);
	}
	animation->Render(x, y, 255, scale);
}

void Point::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + POINT_WIDTH;
	bottom = top + POINT_HEIGHT;
}
