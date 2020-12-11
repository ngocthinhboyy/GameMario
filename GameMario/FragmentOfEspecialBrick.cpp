#include "FragmentOfEspecialBrick.h"
#include "StaticObjectDefine.h"
#include "AnimationDatabase.h"

FragmentOfEspecialBrick::FragmentOfEspecialBrick(float x, float y, float w, float h, float vx, float vy)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->vx = vx;
	this->vy = vy;
	this->gameObjectID = idGenerate++;
	this->timeStart = GetTickCount64();
	this->noCollisionConsideration = true;
}

void FragmentOfEspecialBrick::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(0.75, 0.75);
	animation = AnimationDatabase::GetInstance()->Get(FRAGMENT_ESPECIAL_BRICK_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void FragmentOfEspecialBrick::Update(DWORD dt, int scaleTime)
{
	if (GetTickCount64() - timeStart >= 5000) {
		stillAlive = false;
		return;
	}
	vy += 0.002f * dt;
	GameObject::Update(dt, scaleTime);
	x += dx;
	y += dy;
}

void FragmentOfEspecialBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + w;
	bottom = y + h;
}
