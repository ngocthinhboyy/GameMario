#include "Flower.h"
#include "EnemyDefine.h"
#include "AnimationDatabase.h"
#include "Mario.h"
#include "debug.h"
#include "Fireball.h"
#include "Grid.h"
#include "DynamicObjectDefine.h"
#include "PlayerLevelDownTransformState.h"
#include "PlayScene.h"
#include "PlayerDieState.h"

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
	this->gameObjectID = idGenerate++;
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
				this->animation = animationDatabase->Get(GREEN_FLOWER_ANI_MOVE);
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
		animation->Render(5568, 1104, 255, scalePipe);
		animation = animationDatabase->Get(2601);
		animation->Render(5376, 1152, 255, scalePipe);
	}
	//RenderBoundingBox();
}

void Flower::Update(DWORD dt, int scaleTime)
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
		if (y + h < startPositionY && !finishedAttackingState) {
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
			if (type == RED_FIRE_FLOWER_TYPE || type == GREEN_FIRE_FLOWER_TYPE)
				Attack();
			alreadyAttacked = true;
		}
		else if (timeAttack - startAttacking >= 1500) {
			this->state = FLOWER_STATE_MOVE;
			finishedAttackingState = true;
			alreadyAttacked = false;
		}
	}
	else {
		vy = 0;
		if (x - 24 - 46 >= mario->x || x + 48 + 24 + 47 <= mario->x + 46) {
			int now = GetTickCount64();
			if (type == RED_FIRE_FLOWER_TYPE || type == GREEN_FIRE_FLOWER_TYPE) {
				if (now - lastMoving >= 1000) {
					isMoving = true;
					isGoUp = true;
					finishedAttackingState = false;
					alreadyAttacked = false;
				}
			}
			else if (type == GREEN_FLOWER_TYPE) {
				if (now - lastMoving >= 500) {
					isMoving = true;
					isGoUp = true;
					finishedAttackingState = false;
					alreadyAttacked = false;
				}
			}
		}
		else {
			isMoving = false;
		}
	}
	GameObject::Update(dt,scaleTime);
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
	if (collisionEvent->nx != 0) {
		Mario::GetInstance()->vx = 0;
		if (Mario::GetInstance()->GetLevel() >= 2) {
			Mario::GetInstance()->StartUntouchable();
			Mario::GetInstance()->ChangeState(PlayerLevelDownTransformState::GetInstance());
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			scene->StopGame(1000);
		}
		else if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_SMALL) {
			Mario::GetInstance()->ChangeState(PlayerDieState::GetInstance());
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			Mario::GetInstance()->noCollisionConsideration = true;
			scene->StopGame(5000);
		}
	}
	if (collisionEvent->ny != 0) {
		Mario::GetInstance()->vy = 0;
		if (Mario::GetInstance()->GetLevel() >= 2) {
			Mario::GetInstance()->StartUntouchable();
			Mario::GetInstance()->ChangeState(PlayerLevelDownTransformState::GetInstance());
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			scene->StopGame(1000);
		}
		else if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_SMALL) {
			Mario::GetInstance()->ChangeState(PlayerDieState::GetInstance());
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			Mario::GetInstance()->noCollisionConsideration = true;
			scene->StopGame(5000);
		}
	}
}
