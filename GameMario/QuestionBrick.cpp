#include "QuestionBrick.h"
#include "AnimationDatabase.h"
#include "StaticObjectDefine.h"
#include "debug.h"
#include "Mario.h"
#include "Coin.h"
#include "ItemDefine.h"
#include "Grid.h"

QuestionBrick::QuestionBrick()
{
}

QuestionBrick::QuestionBrick(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->start_Y_position = y;
	this->gameObjectID = idGenerate++;
	this->startPositionX = x;
	this->startPositionY = y;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	animation = AnimationDatabase::GetInstance()->Get(QUESTION_BRICK_ANI);
}

void QuestionBrick::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	if (collisionEvent->ny != 0) {
		if (collisionEvent->ny > 0)
		{
			mario->SetIsOnGround(false);
			if (!isEmptyBrick) {
				Coin* coin = new Coin(x + QUESTION_BRICK_BBOX_WIDTH / 2 - COIN_BBOX_WIDTH / 2, y - 3, COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
				coin->vy = -0.8f;
				Grid::GetInstance()->DeterminedGridToObtainObject(coin);
				isEmptyBrick = true;
			}
		}
		else if (collisionEvent->ny < 0) {
			mario->SetIsOnGround(true);
		}
		mario->vy = 0;
	}
	if (collisionEvent->nx != 0) {
		mario->vx = 0;
	}
}



void QuestionBrick::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if(isEmptyBrick)
		animation = AnimationDatabase::GetInstance()->Get(QUESTION_BRICK_DIE_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void QuestionBrick::Update(DWORD dt)
{

	if (isEmptyBrick && !alreadyMoving) {
		if (vy == 0) {
			vy = -0.03 * dt;
		}
		else {
			if (y >= start_Y_position) {
				vy = 0;
				alreadyMoving = true;
				y = start_Y_position;
			}
			else
				vy += 0.005 *dt;
		}
		GameObject::Update(dt);
		y += dy;
	}
}

void QuestionBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + QUESTION_BRICK_BBOX_WIDTH;
	bottom = top + QUESTION_BRICK_BBOX_HEIGHT;
}
