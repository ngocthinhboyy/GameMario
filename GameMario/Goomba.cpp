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
	SetState(ENEMY_STATE_WALKING);
	
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

void Goomba::Update(DWORD dt)
{
	//DebugOut(L"GOOMBA NE \n");
	vy += ENEMY_GRAVITY * dt;
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	//vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjects = scene->objects;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (!isUpsideDown) {
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
		CalcPotentialCollisions(&coObjects, coEvents);
		//CalcPotentialCollisions(&coEnemies, coEvents);
		//CalcPotentialCollisions(Mario::GetInstance(), coEvents);
	}

	if (coEvents.size() == 0 || isUpsideDown)
	{
		x += dx;
		y += dy;
		float cam_x,cam_y;
		Camera* camera = Camera::GetInstance();
		camera->GetCamPos(cam_x, cam_y);
		/*if (y > (cam_y + SCREEN_HEIGHT))
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
			else if (Tail * tail = dynamic_cast<Tail*> (e->obj)) {
				vx = ENEMY_DIE_SPEED_X;
				if (x < Mario::GetInstance()->x) {
					vx = -ENEMY_DIE_SPEED_X;
				}
				vy = -ENEMY_DIE_SPEED_Y;
				isUpsideDown = true;
			}
			else if (LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
				/*if (dynamic_cast<Koopa*> (enemy))
					enemy->SetState(ENEMY_STATE_DIE);
				vx = 0.2f;
				vy = -0.5f;
				stillAlive = false;*/
			}
			else if (QuestionBrick* brick = dynamic_cast<QuestionBrick*> (e->obj)) {
				//DebugOut(L"AAAA %f\n", e->nx);
				if (e->ny != 0) vy = 0;
				if (e->nx != 0) {
					vx = -vx;
				}
			}
			/*else {
				if (e->ny != 0) vy = 0;
				if (e->nx != 0) {
					vx = -vx;
				}
			}*/
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Goomba::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	/*l = x - GOOMBA_BBOX_WIDTH / 2;
	t = y - GOOMBA_BBOX_HEIGHT / 2;*/
	l = x;
	t = y;
	r = l + GOOMBA_BBOX_WIDTH;
	b = t + GOOMBA_BBOX_HEIGHT;
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
		else
			vy = 0;
	}
}

void Goomba::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	if (collisionEvent->nx != 0) {
		mario->vx = 0;
		if (mario->GetLevel() >= 2) {
			mario->StartUntouchable();
			mario->ChangeState(PlayerLevelDownTransformState::GetInstance());
		}
	}
	if (collisionEvent->ny == -1) {
		state = ENEMY_STATE_DIE;
		mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
		//mario->vx = 0;
		isUpsideDown = true;
	}
}

void Goomba::SetStartPosition()
{
	nx = startNx;
    x = this->startPositionX;
	y = this->startPositionY; 
	isUpsideDown = false;
	vx = -GOOMBA_WALKING_SPEED;
	//vy = 0;
	SetState(ENEMY_STATE_WALKING);
}
