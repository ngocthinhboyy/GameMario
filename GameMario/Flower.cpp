#include "Flower.h"
#include "EnemyDefine.h"
#include "AnimationDatabase.h"
#include "Mario.h"
#include "debug.h"

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
	this->nx = -1;
	this->startNx = -1;
	this->startPositionX = x;
	this->startPositionY = y;
	this->state = FLOWER_STATE_MOVE;
}

void Flower::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	switch (this->type)
	{
		case RED_FIRE_FLOWER_TYPE: {
			if(state == FLOWER_STATE_MOVE)
				this->animation = animationDatabase->Get(RED_FIRE_FLOWER_ANI_MOVE);
			else if(state == FLOWER_STATE_ATTACK)
				this->animation = animationDatabase->Get(RED_FIRE_FLOWER_ANI_ATTACK);
			break;
		}
		case GREEN_FIRE_FLOWER_TYPE: {
			if (state == FLOWER_STATE_MOVE)
				this->animation = animationDatabase->Get(GREEN_FIRE_FLOWER_ANI_MOVE);
			else if (state == FLOWER_STATE_ATTACK)
				this->animation = animationDatabase->Get(GREEN_FIRE_FLOWER_ANI_ATTACK);
			break;
		}
		case GREEN_FLOWER_TYPE: {
			if (state == FLOWER_STATE_MOVE)
				this->animation = animationDatabase->Get(GREEN_FLOWER_ANI_MOVE);
			else if (state == FLOWER_STATE_ATTACK)
				this->animation = animationDatabase->Get(GREEN_FLOWER_ANI_ATTACK);
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
}

void Flower::Update(DWORD dt)
{
	if (isMoving && state == FLOWER_STATE_MOVE) {
		if (isGoUp) {
			vy = -0.08f;
		}
		else {
			vy = 0.08f;
		}
		if (y + 96 < startPositionY && !finishedAttacking) {
			this->state = FLOWER_STATE_ATTACK;
			isGoUp = false;
			startAttacking = GetTickCount64();
			vy = 0;
		}
		if (y > startPositionY) {
			isGoUp = true;
			isMoving = false;
			lastMoving = GetTickCount64();
		}
	}
	else if (state == FLOWER_STATE_ATTACK) {
		int timeAttack = GetTickCount64();
		if (timeAttack - startAttacking >= 1000) {
			this->state = FLOWER_STATE_MOVE;
			finishedAttacking = true;
		}
	}
	else {
		vy = 0;
		int now = GetTickCount64();
		if (now - lastMoving >= 500) {
			isMoving = true;
			isGoUp = true;
			finishedAttacking = false;
		}
	}
	GameObject::Update(dt);
	y += dy;
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
