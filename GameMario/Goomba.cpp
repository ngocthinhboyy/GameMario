#include "Goomba.h"
#include "EnemyDefine.h"
#include "MarioDefine.h"
#include "PlayScene.h"
#include "AnimationDatabase.h"
#include "debug.h"

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
	//this->vx = -GOOMBA_WALKING_SPEED;
	this->type = type;
	SetState(GOOMBA_STATE_WALKING);
	
}

void Goomba::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	if (type == 1) {
		switch (state)
		{
		case GOOMBA_STATE_WALKING:
		{
			animation = animationDatabase->Get(GOOMBA_TAN_ANI_WALKING);
			break;
		}
		case GOOMBA_STATE_DIE:
		{
			animation = animationDatabase->Get(GOOMBA_TAN_ANI_DIE);
			stillAlive = false;
			break;
		}
		default:
			break;
		}
	}
	else if (type == 2) {
		switch (state)
		{
			case GOOMBA_STATE_WALKING:
			{
				animation = animationDatabase->Get(GOOMBA_RED_ANI_WALKING);
				break;
			}
			case GOOMBA_STATE_DIE:
			{
				animation = animationDatabase->Get(GOOMBA_RED_ANI_DIE);
				stillAlive = false;
				break;
			}
			case GOOMBA_STATE_WALKING_WITH_SWINGS:
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
	if(!stillAlive)
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_FLIP_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void Goomba::Update(DWORD dt)
{
	vy += ENEMY_GRAVITY * dt;
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	//vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	//CalcPotentialCollisions(&coEnemies, coEvents);
	if(stillAlive)
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);

	if (coEvents.size() == 0 || !stillAlive)
	{
		x += dx;
		y += dy;
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
			else{
				if (e->nx != 0) vx = 0;
				if (e->ny != 0) vy = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Goomba::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GOOMBA_BBOX_WIDTH / 2;
	t = y - GOOMBA_BBOX_HEIGHT / 2;
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
			vx = 0;
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
