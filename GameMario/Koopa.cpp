#include "Koopa.h"
#include "MarioDefine.h"
#include "PlayScene.h"
#include "EnemyDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "PlayerJumpingState.h"
#include "PlayerKickingState.h"
#include "PlayerHoldingState.h"
#include "Camera.h"
#include "QuestionBrick.h"
#include "StaticObjectDefine.h"
#include "Leaf.h"
#include "ItemDefine.h"
#include "Grid.h"
#include "Coin.h"
#include "PlayerLevelDownTransformState.h"

Koopa::Koopa()
{
}

Koopa::Koopa(float x, float y, float w, float h, int typeKoopa, int typeMove)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = typeKoopa;
	this->gameObjectID = idGenerate++;
	this->nx = -1;
	this->startNx = -1;
	this->startPositionX = x;
	this->startPositionY = y;
	this->startTypeMove = typeMove;
	this->vx = -KOOPA_WALKING_SPEED_X;
	if(typeMove == 1)
		SetState(ENEMY_STATE_WALKING);
	else if (typeMove == 2) {

		SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
	}
}

void Koopa::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	if (vx > 0)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if(isUpsideDown)
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_FLIP_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	//RenderBoundingBox();
}

void Koopa::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	if (type == 1) {
		switch (state)
		{
		case ENEMY_STATE_WALKING:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_WALKING);
			break;
		}
		case ENEMY_STATE_DIE:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_DIE);
			break;
		}
		case ENEMY_STATE_SPIN_DIE_KICK:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_SPIN_DIE_KICK);
			break;
		}
		case ENEMY_STATE_WALKING_WITH_SWINGS:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_WALKING_WITH_SWINGS);
			break;
		}
		default:
			break;
		}
	}
	else if (type == 2) {
		switch (state)
		{
		case ENEMY_STATE_WALKING:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_WALKING);
			break;
		}
		case ENEMY_STATE_DIE:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_DIE);
			break;
		}
		case ENEMY_STATE_SPIN_DIE_KICK:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_SPIN_DIE_KICK);
			break;
		}
		case ENEMY_STATE_WALKING_WITH_SWINGS:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_WALKING_WITH_SWINGS);
			break;
		}
		default:
			break;
		}
	}
}

void Koopa::Update(DWORD dt)
{
	if (isHold) {
		Mario* mario = Mario::GetInstance();
		if (Mario::GetInstance()->nx * vx <= 0) {
			x = mario->x + KOOPA_HOLDING_DISTANCE_TURN_BACK_X * (mario->nx);
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				y = mario->y - KOOPA_HOLDING_DISTANCE_TURN_BACK_Y_MARIO_SMALL;
			else
				y = mario->y - KOOPA_HOLDING_DISTANCE_TURN_BACK_Y;
		}
		vx = mario->vx;
		vy = mario->vy;
	}
	else {
		vy += ENEMY_GRAVITY * dt;
	}
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjs = scene->objects;
	vector<LPGAMEOBJECT> coEnemies = scene->enemies;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (stillAlive && !isUpsideDown) {
		if (state == ENEMY_STATE_SPIN_DIE_KICK || isHold)
		{
			CalcPotentialCollisions(&coEnemies, coEvents);
		}
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
		CalcPotentialCollisions(&coObjs, coEvents);
	}

	if (coEvents.size() == 0 || !stillAlive || isUpsideDown)
	{
			x += dx;
			y += dy;
			float cam_x, cam_y;
			/*Camera* camera = Camera::GetInstance();
			camera->GetCamPos(cam_x, cam_y);
			if (y > (cam_y + SCREEN_HEIGHT))
				stillAlive = false;*/
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
			else if(LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
				if (dynamic_cast<Koopa*> (enemy))
					enemy->SetState(ENEMY_STATE_DIE);
				enemy->vx = ENEMY_DIE_SPEED_X;
				enemy->vy = -ENEMY_DIE_SPEED_Y;
				enemy->SetIsUpsideDown(true);
			}
			else if (QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*> (e->obj)) {
				if (e->ny != 0) {
					vy = 0;
					if (this->state == ENEMY_STATE_WALKING_WITH_SWINGS) {
						vy -= KOOPA_WALKING_WITH_SWINGS_Y;
					}
				}
				if (e->nx != 0) {
					if (state == ENEMY_STATE_SPIN_DIE_KICK) {
						vx = -vx;
						if (!questionBrick->isEmptyBrick) {
							if (questionBrick->GetType() == QUESTION_BRICK_TYPE_HAS_ESPECIAL_ITEM) {
								if (Mario::GetInstance()->GetLevel() != MARIO_LEVEL_SMALL) {
									Leaf* leaf = new Leaf(questionBrick->x + QUESTION_BRICK_BBOX_WIDTH / 2 - LEAF_BBOX_WIDTH / 2, questionBrick->y - 3, LEAF_BBOX_WIDTH, LEAF_BBOX_HEIGHT);
									leaf->vy = -LEAF_SPEED_Y_APPEAR;
									leaf->vx = LEAF_SPEED;
									Grid::GetInstance()->DeterminedGridToObtainObject(leaf);
								}
							}
							else if (questionBrick->GetType() == QUESTION_BRICK_TYPE_HAS_COIN) {
								Coin* coin = new Coin(questionBrick->x + QUESTION_BRICK_BBOX_WIDTH / 2 - COIN_BBOX_WIDTH / 2, questionBrick->y - 3, COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT,1);
								coin->vy = -COIN_SPEED_Y;
								Grid::GetInstance()->DeterminedGridToObtainObject(coin);
							}
						}
						questionBrick->isEmptyBrick = true;
					}
					else {
						vx = -vx;
					}
				}
			}
			/*else {
				if (e->nx != 0) vx = -vx;
				if (e->ny != 0) vy = 0;
			}*/
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Koopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == ENEMY_STATE_DIE || state == ENEMY_STATE_SPIN_DIE_KICK) {
		/*l = x - KOOPA_BBOX_WIDTH / 2;
		t = y - KOOPA_DIE_BBOX_HEIGHT / 2;*/
		l = x;
		t = y;
		r = l + KOOPA_BBOX_WIDTH;
		b = t + KOOPA_DIE_BBOX_HEIGHT;
	}
	else {
		/*l = x - KOOPA_BBOX_WIDTH / 2;
		t = y - KOOPA_BBOX_HEIGHT / 2;*/
		l = x;
		t = y;
		r = l + KOOPA_BBOX_WIDTH;
		b = t + KOOPA_BBOX_HEIGHT;
	}
}

void Koopa::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
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
		else
			vx = -vx;
	}
	if (collisionEvent->ny != 0) {
		if (state == ENEMY_STATE_WALKING) {
			if (x + dx <= collisionMapObject->x || x + dx + KOOPA_BBOX_WIDTH >= collisionMapObject->x + collisionMapObject->w) {
				vx = -vx;
			}
		}
		if (collisionMapObjectDirectionY == 0)
		{
			y += dy;
		}
		else if (collisionEvent->ny < 0 && collisionMapObjectDirectionY == 1) {
			y += dy;
		}
		else if (collisionEvent->ny > 0 && collisionMapObjectDirectionY == -1) {
			y += dy;
		}
		else
		{
			vy = 0;
			if (this->state == ENEMY_STATE_WALKING_WITH_SWINGS) {
				vy -= KOOPA_WALKING_WITH_SWINGS_Y;
			}
		}
	}
}

void Koopa::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	if (collisionEvent->nx != 0) {
		if (!isHold) {
			mario->vx = 0;
			if (state == ENEMY_STATE_DIE) {
				if (mario->GetIsRunning()) {
					isHold = true;
					mario->ChangeState(PlayerHoldingState::GetInstance());
				}
				else {
						mario->ChangeState(PlayerKickingState::GetInstance());
						if (collisionEvent->nx > 0) {
							vx = -KOOPA_SPEED_TORTOISESHELL;
							state = ENEMY_STATE_SPIN_DIE_KICK;
						}
						else {
							vx = KOOPA_SPEED_TORTOISESHELL;
							state = ENEMY_STATE_SPIN_DIE_KICK;
						}
				}
			}
			else {
				if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
					mario->StartUntouchable();
					mario->ChangeState(PlayerLevelDownTransformState::GetInstance());
					mario->vx = 0;
					mario->vy = 0;
			}
			}
		}
	}
	if (collisionEvent->ny != 0) {
		if (collisionEvent->ny < 0) {
			if (state != ENEMY_STATE_DIE) {
				state = ENEMY_STATE_DIE;
				mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
				vx = 0;
			}
			else if (state == ENEMY_STATE_DIE) {
				mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
				if (mario->x > x)
					vx = -KOOPA_SPEED_TORTOISESHELL;
				else if (mario->x <= x)
					vx = KOOPA_SPEED_TORTOISESHELL;
				state = ENEMY_STATE_SPIN_DIE_KICK;
			}
		}
		if (collisionEvent->ny > 0) {
			if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
				mario->StartUntouchable();
				mario->ChangeState(PlayerLevelDownTransformState::GetInstance());
				mario->vx = 0;
				mario->vy = 0;
			}
		}
	}
}

void Koopa::SetStartPosition()
{
	this->x = this->startPositionX;
	this->y = this->startPositionY;
	this->nx = this->startNx;
	this->vx = -KOOPA_WALKING_SPEED_X;
	this->isUpsideDown = false;
	if (startTypeMove == 1) {
		SetState(ENEMY_STATE_WALKING);
	}
	else if (startTypeMove == 2) {

		SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
	}
}
