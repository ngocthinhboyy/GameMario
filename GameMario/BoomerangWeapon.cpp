#include "BoomerangWeapon.h"
#include "AnimationDatabase.h"
#include "Mario.h"
#include "BoomerangBrother.h"
#include "PlayerLevelDownTransformState.h"
#include "PlayScene.h"
#include "PlayerDieState.h"

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
	this->startPositionX = x - 10;
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
	//RenderBoundingBox();
}

void BoomerangWeapon::ChangeState()
{
}

void BoomerangWeapon::Update(DWORD dt, int scaleTime)
{
	DWORD now = GetTickCount64();
	if (now - timeTurnBack >= 600 && !isTurningBack) {
		vy = vy + 0.02f;
		vx = 0.2f;
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
	if (x < startPositionX) {
		this->stillAlive = false;
	}
}

void BoomerangWeapon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + w;
	b = t + h;
}

void BoomerangWeapon::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
{
}

void BoomerangWeapon::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	if (mario->GetUntouchable()) {
		if (collisionEvent->nx != 0)
			mario->x += mario->dx;
		if (collisionEvent->ny != 0)
		{
			if (collisionEvent->ny > 0)
				mario->y -= mario->dy;
			if (collisionEvent->ny < 0)
				mario->y += mario->dy;
		}
	}
	else
	{
		mario->y -= 2;
		if (collisionEvent->ny > 0)
			mario->y -= mario->dy;
		if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
			mario->StartUntouchable();
			noCollisionConsideration = true;
			mario->ChangeState(PlayerLevelDownTransformState::GetInstance());
			scene->StopGame(1000);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			mario->y -= 15;
			mario->ChangeState(PlayerDieState::GetInstance());
			mario->noCollisionConsideration = true;
			mario->vx = 0;
			scene->StopGame(5000);
		}
	}
}
