#include "BoomerangWeapon.h"
#include "AnimationDatabase.h"
#include "Mario.h"

BoomerangWeapon::BoomerangWeapon()
{
}

BoomerangWeapon::BoomerangWeapon(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->startPositionY = y + 10;
	this->gameObjectID = idGenerate++;
	this->vx = 0.4f;
	this->vy = -0.2f;
	this->timeTurnBack = GetTickCount64();
}

void BoomerangWeapon::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	animation = animationDatabase->Get(4602);
}

void BoomerangWeapon::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void BoomerangWeapon::ChangeState()
{
}

void BoomerangWeapon::Update(DWORD dt, int scaleTime)
{
	DWORD now = GetTickCount64();
	if (now - timeTurnBack >= 600 && !isTurningBack) {
		vy = vy + 0.02f;
		vx = 0.3f;
		if(y > startPositionY)
			isTurningBack = true;
	}
	if (isTurningBack && !isAlreadyTurnedBack) {
		vx = -0.4f;
		vy = 0;
		isAlreadyTurnedBack = true;
	}
	GameObject::Update(dt, scaleTime);
	x += dx;
	y += dy;
}

void BoomerangWeapon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void BoomerangWeapon::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
{
}

void BoomerangWeapon::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
}
