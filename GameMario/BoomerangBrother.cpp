#include "BoomerangBrother.h"
#include "AnimationDatabase.h"
#include "EnemyDefine.h"
#include "PlayScene.h"
#include "debug.h"
#include "BoomerangWeapon.h"
#include "Grid.h"

BoomerangBrother::BoomerangBrother()
{
}

BoomerangBrother::BoomerangBrother(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->nx = 1;
	this->gameObjectID = idGenerate++;
}

void BoomerangBrother::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	switch (state)
	{
		case BOOMERANG_BROTHER_MOVING_STATE: {
				animation = animationDatabase->Get(4601);
				break;
			}
		case BOOMERANG_BROTHER_ATTACK_STATE: {
			animation = animationDatabase->Get(4603);
			break;
		}
		default:
			break;
	}
}

void BoomerangBrother::Render()
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

void BoomerangBrother::ChangeState()
{
	DWORD now = GetTickCount64();
	if (timeMove == 0) {
		timeMove = now;
		timeAttack = now;
		vx = 0.09f;
		state = BOOMERANG_BROTHER_MOVING_STATE;
	}
	else if (now - timeMove >= 1000) {
		vx = -vx;
		timeMove = now;
		state = BOOMERANG_BROTHER_MOVING_STATE;
	}
	else if (state == BOOMERANG_BROTHER_ATTACK_STATE && now - timeAttack >= 200) {
		state = BOOMERANG_BROTHER_MOVING_STATE;
	}
	if (now - timeAttack >= 2000) {
		state = BOOMERANG_BROTHER_ATTACK_STATE;
		BoomerangWeapon* boomerang = new BoomerangWeapon(x + w, y + 5, 45,45);
		Grid::GetInstance()->DeterminedGridToObtainObject(boomerang);
		timeAttack = now;
	}
}

void BoomerangBrother::Update(DWORD dt, int scaleTime)
{
	vy += ENEMY_GRAVITY * dt;
	ChangeState();
	GameObject::Update(dt, scaleTime);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		int coEventsSize = coEventsResult.size();

		for (UINT i = 0; i < coEventsSize; i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (LPCOLLISIONMAPOBJECT collMapObj = dynamic_cast<LPCOLLISIONMAPOBJECT> (e->obj)) {
				CollisionWithCollisionMapObject(e, collMapObj);
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BoomerangBrother::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + w;
	b = t + h;
}

void BoomerangBrother::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
{
	int collisionMapObjectDirectionX = collisionMapObject->GetCollisionDirectionX();
	int collisionMapObjectDirectionY = collisionMapObject->GetCollisionDirectionY();
	if (collisionEvent->nx != 0) {
		if (collisionMapObjectDirectionX == 0)
			x += dx;
		else if (collisionEvent->nx < 0 && collisionMapObjectDirectionX == 1)
			x += dx;
		else if (collisionEvent->nx > 0 && collisionMapObjectDirectionX == -1)
			x += dx;
		else {
			vx = -vx;
		}
	}
	if (collisionEvent->ny != 0) {
		if (collisionMapObjectDirectionY == 0)
			y += dy;
		else if (collisionEvent->ny < 0 && collisionMapObjectDirectionY == 1)
			y += dy;
		else if (collisionEvent->ny > 0 && collisionMapObjectDirectionY == -1)
			y += dy;
		else {
			vy = 0; 
		}
	}
}

void BoomerangBrother::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
}
