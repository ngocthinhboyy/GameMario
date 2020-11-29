#include "Flower.h"
#include "EnemyDefine.h"
#include "AnimationDatabase.h"
#include "Mario.h"
#include "debug.h"
#include "Fireball.h"
#include "Grid.h"
#include "DynamicObjectDefine.h"

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
			if(Mario::GetInstance()->y < y)
				this->animation = animationDatabase->Get(RED_FIRE_FLOWER_ANI_LOOK_DOWN);
			else
				this->animation = animationDatabase->Get(RED_FIRE_FLOWER_ANI_LOOK_UP);
			break;
		}
		case GREEN_FIRE_FLOWER_TYPE: {
			if (Mario::GetInstance()->y < y)
				this->animation = animationDatabase->Get(GREEN_FIRE_FLOWER_ANI_LOOK_DOWN);
			else
				this->animation = animationDatabase->Get(GREEN_FIRE_FLOWER_ANI_LOOK_UP);
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
	if (nx > 0)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
		D3DXVECTOR2 scalePipe;
		scalePipe = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
		AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
		animation = animationDatabase->Get(2101);
		animation->Render(1056, 1104, 255, scalePipe);
	}
}

void Flower::Update(DWORD dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->x > x)
		nx = 1;
	else
		nx = -1;
	if (isMoving && state == FLOWER_STATE_MOVE) {
		if (isGoUp) {
			vy = -0.1f;
		}
		else {
			vy = 0.1f;
		}
		if (y + 96 < startPositionY && !finishedAttackingState) {
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
		if (timeAttack - startAttacking >= 1000 && !alreadyAttacked) {
			Attack();
			alreadyAttacked = true;
		}
		else if(timeAttack - startAttacking >= 1500) {
			this->state = FLOWER_STATE_MOVE;
			finishedAttackingState = true;
			alreadyAttacked = false;
		}
	}
	else {
		vy = 0;
		if (x - 24 - 46 >= mario->x || x + 48 + 24 + 47 <= mario->x + 46) {
			int now = GetTickCount64();
			if (now - lastMoving >= 1000) {
				isMoving = true;
				isGoUp = true;
				finishedAttackingState = false;
				alreadyAttacked = false;
			}
		}
		else {
			isMoving = false;
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

void Flower::Attack()
{
	Mario* mario = Mario::GetInstance();
	Fireball* fireball = new Fireball(x + (FLOWER_BBOX_WIDTH / 2 - FIREBALL_BBOX_WIDTH / 2), y + 5, FIREBALL_WIDTH, FIREBALL_HEIGHT, FIREBALL_TYPE_OF_FLOWER);
	if (mario->y < y) {
		fireball->vy = -0.1f;
	}
	else
		fireball->vy = 0.1f;
	if (mario->x <= x - 220 || mario->x >= x + 220) {
		fireball->vx = 0.27 * nx;
	}
	else {
		fireball->vx = 0.13 * nx;
	}
	Grid* grid = Grid::GetInstance();
	grid->DeterminedGridToObtainObject(fireball);
}

void Flower::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
{
}

void Flower::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
}
