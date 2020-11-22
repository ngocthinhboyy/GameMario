#include "QuestionBrick.h"
#include "AnimationDatabase.h"
#include "StaticObjectDefine.h"
#include "debug.h"
#include "Mario.h"
#include "Coin.h"
#include "ItemDefine.h"
#include "Grid.h"
#include "Leaf.h"
#include "Mushroom.h"
#include "PlayerRunningState.h"

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
	this->vx = 0;
	this->y = y;
	this->w = w;
	this->h = h;
	if (type == 1)
		this->type = QUESTION_BRICK_TYPE_HAS_COIN;
	else if (type == 2)
		this->type = QUESTION_BRICK_TYPE_HAS_ESPECIAL_ITEM;

	animation = AnimationDatabase::GetInstance()->Get(QUESTION_BRICK_ANI);
}

void QuestionBrick::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	//if (collisionEvent->nx != 0) {
	//	DebugOut(L"AAA %d\n", mario->nx);
	//	DebugOut(L"BBB %f\n", mario->vx);
	//	PlayerRunningState::lastStateIsSkidding = true;
	//	Mario* mario = Mario::GetInstance();
	//	mario->vx += (dt * MARIO_SPEED_ACCELERATION * 3.5 * -(mario->nx));
	//}
	if (collisionEvent->ny != 0) {
		if (collisionEvent->ny > 0)
		{
			mario->SetIsOnGround(false);
			if (!isEmptyBrick) {
				if (type == QUESTION_BRICK_TYPE_HAS_COIN)
				{
					Coin* coin = new Coin(x + QUESTION_BRICK_BBOX_WIDTH / 2 - COIN_BBOX_WIDTH / 2, y - 3, COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
					coin->vy = -COIN_SPEED_Y;
					mario->SetCoin(mario->GetCoin()+1);
					Grid::GetInstance()->DeterminedGridToObtainObject(coin);
				}
				else if (type == QUESTION_BRICK_TYPE_HAS_ESPECIAL_ITEM) {
					if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
						Leaf* leaf = new Leaf(x + QUESTION_BRICK_BBOX_WIDTH / 2 - LEAF_BBOX_WIDTH / 2, y - 3, LEAF_BBOX_WIDTH, LEAF_BBOX_HEIGHT);
						leaf->vy = -LEAF_SPEED_Y_APPEAR;
						leaf->vx = LEAF_SPEED;
						Grid::GetInstance()->DeterminedGridToObtainObject(leaf);
					}
				}
				isEmptyBrick = true;
			}
		}
		else if (collisionEvent->ny < 0) {
			mario->SetIsOnGround(true);
		}
		mario->vy = 0;
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
				if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_SMALL && type == QUESTION_BRICK_TYPE_HAS_ESPECIAL_ITEM)
				{
					Mushroom* mushroom = new Mushroom(x + QUESTION_BRICK_BBOX_WIDTH / 2 - MUSHROOM_BBOX_WIDTH / 2, y, MUSHROOM_BBOX_WIDTH, MUSHROOM_BBOX_HEIGHT);
					mushroom->vy = -MUSHROOM_SPEED_Y_APPEAR;
					Grid::GetInstance()->DeterminedGridToObtainObject(mushroom);
				}
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
