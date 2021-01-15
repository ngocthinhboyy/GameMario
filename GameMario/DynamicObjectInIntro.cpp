#include "DynamicObjectInIntro.h"
#include "Platform.h"
#include "MarioDefine.h"
#include "IntroMap.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "IntroMapDefine.h"
#include "ItemDefine.h"

DynamicObjectInIntro::DynamicObjectInIntro(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->startPositionX = x;
	this->startPositionY = y;
	this->type = type;
}

void DynamicObjectInIntro::Update(DWORD dt, int scaleTime)
{
	vy += MARIO_GRAVITY * dt;
	if (type == TYPE_LEAF) {
		if (IsOverlapWithMario()) {
			stillAlive = false;
			IntroMap* scene = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());
			scene->marioRed->SetState(MARIO_IN_INTRO_STATE_RACCOON_FALLING);
		}
		if (y < startPositionY) {
			vy = ITEM_GRAVITY * dt * 3.5;
		}
		else {
			vy = 0;
			vy = ITEM_GRAVITY * dt * 3.5;
		}
		if (isSupplyFirstSpeed) {
			vx = LEAF_SPEED_X;
			isSupplyFirstSpeed = false;
		}
		if (x >= startPositionX + 50) {
			vx = -LEAF_SPEED_X;
		}
		else if (x <= startPositionX - 50) {
			vx = LEAF_SPEED_X;
		}
	}
	GameObject::Update(dt, scaleTime);

	IntroMap* scene = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coObjects = scene->objects;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&coObjects, coEvents);
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
			else if (Platform * platform = dynamic_cast<Platform*> (e->obj)) {
				x += dx;
				y += dy;
			}

		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void DynamicObjectInIntro::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (nx < 0)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else if (nx > 0)
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	SetAnimation();
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void DynamicObjectInIntro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + w;
	bottom = y + h;
}

void DynamicObjectInIntro::SetAnimation()
{
	if (type == TYPE_ENEMY_KOOPA_TORTOISE) {
		animation = AnimationDatabase::GetInstance()->Get(KOOPA_TORTOISE_ANI);
	}
	else if (type == TYPE_LEAF) {
		animation = AnimationDatabase::GetInstance()->Get(LEAF_ANI);
	}
	else if (type == TYPE_ENEMY_GOOMBA) {
		animation = AnimationDatabase::GetInstance()->Get(GOOMBA_ANI);
	}
}

void DynamicObjectInIntro::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
{
}

void DynamicObjectInIntro::CollisionWithMario(LPCOLLISIONEVENT collisionEvent, DynamicObjectInIntro* object)
{
	if (collisionEvent->ny > 0) {
		if (type == TYPE_ENEMY_KOOPA_TORTOISE) {
			object->vx = -0.1f;
			object->vy = -0.7f;
			IntroMap* scene = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());
			scene->marioRed->y -= 2;
			scene->marioRed->SetState(MARIO_IN_INTRO_STATE_CRACKING_NECK);
			scene->marioRed->SetTimeJumping();
		}
	}
}

bool DynamicObjectInIntro::IsOverlapWithMario()
{
	float leftBB, rightBB, topBB, bottomBB = .0f;
	float leftBBMario, rightBBMario, topBBMario, bottomBBMario = .0f;
	GetBoundingBox(leftBB, topBB, rightBB, bottomBB);
	IntroMap* scene = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());
	scene->marioRed->GetBoundingBox(leftBBMario, topBBMario, rightBBMario, bottomBBMario);
	float widthBB = rightBB - leftBB;
	float heightBB = bottomBB - topBB;
	float widthMario = rightBBMario - leftBBMario;
	float heightMario = bottomBBMario - topBBMario;

	if ((leftBB + widthBB >= leftBBMario) && (leftBBMario + widthMario >= leftBB) && (topBB + heightBB >= topBBMario) && (topBBMario + heightMario >= topBB))
		return true;
	return false;
}

DynamicObjectInIntro::~DynamicObjectInIntro()
{
}
