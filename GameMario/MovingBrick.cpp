#include "MovingBrick.h"
#include "AnimationDatabase.h"
#include "StaticObjectDefine.h"
#include "Mario.h"
#include "EnemyDefine.h"
#include "DynamicObjectDefine.h"
#include "debug.h"

MovingBrick::MovingBrick(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->vx = -MOVING_BRICK_SPEED;
}

void MovingBrick::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{

	if (collisionEvent->ny != 0) {
		if (collisionEvent->ny > 0)
		{
			Mario::GetInstance()->SetIsOnGround(false);

		}
		else if (collisionEvent->ny < 0) {
			isStanded = true;
			vx = 0;
			Mario::GetInstance()->SetIsOnGround(true);
		}
		Mario::GetInstance()->SetCanJump(true);
		Mario::GetInstance()->vy = 0;
	}
	if (collisionEvent->nx != 0) {
		Mario::GetInstance()->vx = vx;
	}
}

void MovingBrick::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	animation = AnimationDatabase::GetInstance()->Get(MOVING_BRICK_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void MovingBrick::Update(DWORD dt, int scaleTime)
{
	if(isStanded)
		vy += ENEMY_GRAVITY * dt;
	GameObject::Update(dt, scaleTime);
	x += dx;
	y += dy;
}

void MovingBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
