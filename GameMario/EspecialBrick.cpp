#include "EspecialBrick.h"
#include "AnimationDatabase.h"
#include "StaticObjectDefine.h"
#include "Mario.h"
#include "FragmentOfEspecialBrick.h"
#include "Grid.h"
#include "ButtonP.h"
#include "Mushroom.h"
#include "ItemDefine.h"
#include "Coin.h"
#include "Leaf.h"

EspecialBrick::EspecialBrick(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->startPositionY = y;
	this->type = type;
	this->gameObjectID = idGenerate++;
	this->noCollisionConsideration = false;
}

void EspecialBrick::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	if (collisionEvent->ny != 0) {
		if (collisionEvent->ny > 0)
		{
			mario->SetIsOnGround(false);
			if (type == ESPECIAL_BRICK_TYPE_HAS_EMPTY) {
				stillAlive = false;
				FragmentOfEspecialBrick* fragment1 = new FragmentOfEspecialBrick(x, y, 39, 39, -0.15, -1.1);
				FragmentOfEspecialBrick* fragment2 = new FragmentOfEspecialBrick(x, y, 39, 39, 0.15, -1.1);
				FragmentOfEspecialBrick* fragment3 = new FragmentOfEspecialBrick(x, y, 39, 39, -0.15, -0.8);
				FragmentOfEspecialBrick* fragment4 = new FragmentOfEspecialBrick(x, y, 39, 39, 0.15, -0.8);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment1);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment2);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment3);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment4);
			}
			else if (type == ESPECIAL_BRICK_TYPE_HAS_BUTTON_P) {
				if (!isEmptyBrick) {
					isEmptyBrick = true;
					ButtonP* buttonP = new ButtonP(x, y, w, h);
					Grid::GetInstance()->DeterminedGridToObtainObject(buttonP);
				}
			}
			else if (type == ESPECIAL_BRICK_TYPE_HAS_1UP) {
				if (!isEmptyBrick) {
					isEmptyBrick = true;
					Mushroom* mushroom = new Mushroom(x, y, MUSHROOM_BBOX_WIDTH, MUSHROOM_BBOX_HEIGHT, MUSHROOM_TYPE_GREEN);
					mushroom->vy = -MUSHROOM_SPEED_Y_APPEAR;
					Grid::GetInstance()->DeterminedGridToObtainObject(mushroom);
				}
			}
			else if (type == ESPECIAL_BRICK_TYPE_HAS_MANY_COINS) {
				if (countCoinRemaining > 0) {
					countCoinRemaining--;
					isEmptyBrick = true;
					alreadyMoving = false;
					Coin* coin = new Coin(x + QUESTION_BRICK_BBOX_WIDTH / 2 - COIN_BBOX_WIDTH / 2, y - 3, COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT, COIN_TYPE_FROM_QUESTION_BRICK);
					coin->vy = -COIN_SPEED_Y;
					mario->SetCoin(mario->GetCoin() + 1);
					Grid::GetInstance()->DeterminedGridToObtainObject(coin);
				}
			}
			else if (type == ESPECIAL_BRICK_TYPE_HAS_ITEM) {
				if (!isEmptyBrick) {
					if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
						Leaf* leaf = new Leaf(x + QUESTION_BRICK_BBOX_WIDTH / 2 - LEAF_BBOX_WIDTH / 2, y - 3, LEAF_BBOX_WIDTH, LEAF_BBOX_HEIGHT);
						leaf->vy = -LEAF_SPEED_Y_APPEAR;
						leaf->vx = LEAF_SPEED;
						Grid::GetInstance()->DeterminedGridToObtainObject(leaf);
						isEmptyBrick = true;
					}
				}
			}
		}
		else if (collisionEvent->ny < 0) {
			mario->SetIsOnGround(true);
		}
		mario->vy = 0;
	}
}

void EspecialBrick::Render()
{
	int alpha = 200;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	animation = AnimationDatabase::GetInstance()->Get(ESPECIAL_BRICK_ANI);
	if (((type == ESPECIAL_BRICK_TYPE_HAS_BUTTON_P || type == ESPECIAL_BRICK_TYPE_HAS_1UP || type == ESPECIAL_BRICK_TYPE_HAS_ITEM) && isEmptyBrick) ||(type == ESPECIAL_BRICK_TYPE_HAS_MANY_COINS && countCoinRemaining == 0)) {
		animation = AnimationDatabase::GetInstance()->Get(QUESTION_BRICK_DIE_ANI);
	}
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void EspecialBrick::Update(DWORD dt, int scaleTime)
{
	if (type == ESPECIAL_BRICK_TYPE_HAS_MANY_COINS || type == ESPECIAL_BRICK_TYPE_HAS_ITEM || type == ESPECIAL_BRICK_TYPE_HAS_1UP) {
		if (isEmptyBrick && !alreadyMoving) {
			if (vy == 0) {
				vy = -0.03 * dt;
			}
			else {
				if (y >= startPositionY) {
					vy = 0;
					alreadyMoving = true;
					y = startPositionY;
					if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_SMALL && type == QUESTION_BRICK_TYPE_HAS_ESPECIAL_ITEM)
					{
						Mushroom* mushroom = new Mushroom(x, y, MUSHROOM_BBOX_WIDTH, MUSHROOM_BBOX_HEIGHT, MUSHROOM_TYPE_RED);
						mushroom->vy = -MUSHROOM_SPEED_Y_APPEAR;
						Grid::GetInstance()->DeterminedGridToObtainObject(mushroom);
					}
				}
				else
					vy += 0.0075 * dt;
			}
			GameObject::Update(dt, scaleTime);
			y += dy;
		}
	}
}

void EspecialBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
