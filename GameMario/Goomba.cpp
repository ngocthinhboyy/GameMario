#include "Goomba.h"
#include "EnemyDefine.h"
#include "MarioDefine.h"
#include "PlayScene.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "Camera.h"
#include "QuestionBrick.h"
#include "Tail.h"
#include "PlayerLevelDownTransformState.h"
#include "Point.h"
#include "Grid.h"
#include "PlayerDieState.h"

Goomba::Goomba()
{
}

Goomba::Goomba(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->gameObjectID = idGenerate++;
	this->nx = -1;
	this->startNx = -1;
	this->startPositionX = x;
	this->startPositionY = y;
	this->vx = -GOOMBA_WALKING_SPEED;
	this->type = type;
	if (type == 1)
		SetState(ENEMY_STATE_WALKING);
	else if (type == 2) {
		timeWalkingType2 = GetTickCount64();
		SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
	}
	
}

void Goomba::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	if (type == 1) {
		switch (state)
		{
		case ENEMY_STATE_WALKING:
		{
			animation = animationDatabase->Get(GOOMBA_TAN_ANI_WALKING);
			break;
		}
		case ENEMY_STATE_DIE:
		{
			animation = animationDatabase->Get(GOOMBA_TAN_ANI_DIE);
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
				animation = animationDatabase->Get(GOOMBA_RED_ANI_WALKING);
				break;
			}
			case ENEMY_STATE_DIE:
			{
				animation = animationDatabase->Get(GOOMBA_RED_ANI_DIE);
				break;
			}
			case ENEMY_STATE_WALKING_WITH_SWINGS:
			{
				animation = animationDatabase->Get(GOOMBA_RED_ANI_WALKING_WITH_SWINGS);
				break;
			}
			case RED_GOOMBA_STATE_JUMP:
			{
				animation = animationDatabase->Get(GOOMBA_RED_JUMPING_ANI);
				break;
			}
			case RED_GOOMBA_STATE_HIGH_JUMP:
			{
				animation = animationDatabase->Get(GOOMBA_RED_JUMPING_ANI);
				break;
			}
			default:
				break;
		}
	}
}

void Goomba::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	if (nx < 1)
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

void Goomba::Update(DWORD dt, int scaleTime)
{
	if (state == ENEMY_STATE_DIE) {
		if (GetTickCount64() - timeDie >= 200) {
			stillAlive = false;
		}
		return;
	}
	if (type == 2) {
		ChangeStateRedGoomba();
	}
	vy += ENEMY_GRAVITY * dt;
	GameObject::Update(dt, scaleTime);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	//vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjects = scene->objects;
	vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (!isUpsideDown) {
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
		CalcPotentialCollisions(&coObjects, coEvents);
		CalcPotentialCollisions(&coEnemies, coEvents);
		//CalcPotentialCollisions(Mario::GetInstance(), coEvents);
	}
	if (coEvents.size() == 0 || isUpsideDown)
	{
		x += dx;
		y += dy;
		float cam_x,cam_y;
		Camera* camera = Camera::GetInstance();
		camera->GetCamPos(cam_x, cam_y);
		if (y > (cam_y + SCREEN_HEIGHT) && (state == ENEMY_STATE_DIE || isUpsideDown)) {
			stillAlive = false;
		}
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
			else if (Tail * tail = dynamic_cast<Tail*> (e->obj)) {
				isUpsideDown = true;
				tail->SetHasEffect(true);
				tail->noCollisionConsideration = true;
				if (type == 2)
					state = ENEMY_STATE_WALKING;
				if (x < Mario::GetInstance()->x) {
					vx = -ENEMY_DIE_SPEED_X;
				}
				else
					vx = ENEMY_DIE_SPEED_X;
				vy = -ENEMY_DIE_SPEED_Y;
				noCollisionConsideration = true;
				Point* point = new Point(x, y, 39, 30, 100);
				Grid* grid = Grid::GetInstance();
				grid->DeterminedGridToObtainObject(point);
				Mario::GetInstance()->SetPoint(Mario::GetInstance()->GetPoint() + 100);
			}
			else if (LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
				if (enemy->state != ENEMY_STATE_SPIN_DIE_KICK) {
					if (e->nx != 0) {
						vx = -vx;
						enemy->vx = -enemy->vx;
					}
					if (e->ny != 0) {
						vy = 0;
					}
				}
			}
			else if (QuestionBrick* brick = dynamic_cast<QuestionBrick*> (e->obj)) {
				if (e->ny != 0) vy = 0;
				if (e->nx != 0) {
					vx = -vx;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Goomba::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	/*l = x - GOOMBA_BBOX_WIDTH / 2;
	t = y - GOOMBA_BBOX_HEIGHT / 2;*/
	if (type == 1) {
		l = x;
		t = y;
		r = l + w;
		b = t + h;
	}
	else if (type == 2) {
		l = x;
		t = y;
		r = l + w;
		b = t + h;
	}
}

void Goomba::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
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
			if (type == 2) {
				if (state == RED_GOOMBA_STATE_JUMP) {
					if (countTimeJump < 2) {
						timeJumpType2 = GetTickCount64();
						countTimeJump++;
					}
					else if (countTimeJump == 2) {
						state = RED_GOOMBA_STATE_HIGH_JUMP;
						timeJumpType2 = GetTickCount64();
						countTimeJump = 0;
					}
				}
				else if (state == RED_GOOMBA_STATE_HIGH_JUMP) {
					state = ENEMY_STATE_WALKING_WITH_SWINGS;
					vy = 0;
					timeJumpType2 = 0;
					timeWalkingType2 = GetTickCount64();
					countTimeJump = 0;
				}
				else
					vy = 0;
			}
			else if (type == 1)
				vy = 0;
		}
	}
}

void Goomba::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	if (collisionEvent->nx != 0) {
		mario->vx = 0;
		if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
			mario->StartUntouchable();
			mario->y -= 5;
			mario->ChangeState(PlayerLevelDownTransformState::GetInstance());
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			scene->StopGame(1000);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			mario->ChangeState(PlayerDieState::GetInstance());
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			mario->noCollisionConsideration = true;
			scene->StopGame(5000);
		}
	}
	if (collisionEvent->ny == -1) {
		if (type == 1) {
			state = ENEMY_STATE_DIE;
			mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
			Point* point = new Point(x, y, 39, 30, 100);
			Grid* grid = Grid::GetInstance();
			grid->DeterminedGridToObtainObject(point);
			mario->SetPoint(mario->GetPoint() + 100);
			h = 24;
			vx = 0;
			y += 24;
			timeDie = GetTickCount64();
		}
		else if (type == 2) {
			if (state != ENEMY_STATE_WALKING) {
				state = ENEMY_STATE_WALKING;
				mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
				Point* point = new Point(x, y, 39, 30, 100);
				Grid* grid = Grid::GetInstance();
				grid->DeterminedGridToObtainObject(point);
				mario->SetPoint(mario->GetPoint() + 100);
				y -= 2;
				w = 48;
				h = 48;
			}
			else if (state == ENEMY_STATE_WALKING) {
				state = ENEMY_STATE_DIE;
				mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
				Point* point = new Point(x, y, 39, 30, 100);
				Grid* grid = Grid::GetInstance();
				grid->DeterminedGridToObtainObject(point);
				mario->SetPoint(mario->GetPoint() + 100);
				h = 24;
				vx = 0;
				y += 24;
				timeDie = GetTickCount64();
			}
		}
	}
}

void Goomba::SetStartPosition()
{
	this->nx = startNx;
    this->x = this->startPositionX;
	this->y = this->startPositionY; 
	this->isUpsideDown = false;
	this->vx = -GOOMBA_WALKING_SPEED;
	/*if (Mario::GetInstance()->x > this->startPositionX)
		this->vx = GOOMBA_WALKING_SPEED;
	else
		this->vx = -GOOMBA_WALKING_SPEED;*/
	if (type == 1)
		SetState(ENEMY_STATE_WALKING);
	else if (type == 2) {
		timeWalkingType2 = GetTickCount64();
		SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
		w = 60;
	}
}

void Goomba::ChangeStateRedGoomba()
{
	DWORD now = GetTickCount64();
	if (state == ENEMY_STATE_WALKING_WITH_SWINGS && now - timeWalkingType2 >= 1000) {
		state = RED_GOOMBA_STATE_JUMP;
		timeJumpType2 = now;
		timeWalkingType2 = 0;
	}
	if (state == ENEMY_STATE_WALKING_WITH_SWINGS) {
		h = 57;
		if (Mario::GetInstance()->x < x)
			vx = -GOOMBA_WALKING_SPEED;
		else
			vx = GOOMBA_WALKING_SPEED;
	}
	else if (state == RED_GOOMBA_STATE_JUMP) {
		h = 72;
		vy = -0.2f;
		if (now - timeJumpType2 >= 150) {
			vy = 0.17f;
		}
	}
	else if (state == RED_GOOMBA_STATE_HIGH_JUMP) {
		h = 72;
		vy = -0.2f;
		if (now - timeJumpType2 >= 600) {
			vy = 0.17f;
		}
	}
}
