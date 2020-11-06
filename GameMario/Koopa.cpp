#include "Koopa.h"
#include "MarioDefine.h"
#include "PlayScene.h"
#include "EnemyDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "PlayerJumpingState.h"
#include "PlayerKickingState.h"

Koopa::Koopa()
{
}

Koopa::Koopa(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	//SetState(KOOPA_RED_STATE_WALKING);
	this->type = type;
	SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
}

void Koopa::Render()
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
	vy += ENEMY_GRAVITY * dt;
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjs = scene->objects;
	vector<LPGAMEOBJECT> coEnemies = scene->enemies;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (stillAlive) {
		if (state == ENEMY_STATE_SPIN_DIE_KICK)
			CalcPotentialCollisions(&coEnemies, coEvents);
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
		CalcPotentialCollisions(&coObjs, coEvents);
	}

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
			else if(LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
				if (dynamic_cast<Koopa*> (enemy))
					enemy->SetState(ENEMY_STATE_DIE);
				enemy->vx = 0.2f;
				enemy->vy = -0.5f;
				enemy->stillAlive = false;
			}
			else {
				if (e->nx != 0) vx = -vx;
				if (e->ny != 0) vy = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Koopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == ENEMY_STATE_DIE || state == ENEMY_STATE_SPIN_DIE_KICK) {
		l = x - KOOPA_BBOX_WIDTH / 2;
		t = y - KOOPA_DIE_BBOX_HEIGHT / 2;
		r = l + KOOPA_BBOX_WIDTH;
		b = t + KOOPA_DIE_BBOX_HEIGHT;
	}
	else {
		l = x - KOOPA_BBOX_WIDTH / 2;
		t = y - KOOPA_BBOX_HEIGHT / 2;
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

void Koopa::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	if (collisionEvent->nx != 0) {
		//mario->stillAlive = false;
		if (state == ENEMY_STATE_DIE) {
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
	if (collisionEvent->ny == -1) {
		if (state != ENEMY_STATE_DIE) {
			state = ENEMY_STATE_DIE;
			mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
			/*if (mario->x > x)
				mario->vx = 0.2f;
			else if (mario->x < x)
				mario->vx = -0.2f;
			else*/
				//mario->vx = 0;
			vx = 0;
		}
		else if(state == ENEMY_STATE_DIE) {
			mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
			if (mario->x > x)
				vx = -KOOPA_SPEED_TORTOISESHELL;
			else if (mario->x <= x)
				vx = KOOPA_SPEED_TORTOISESHELL;
			state = ENEMY_STATE_SPIN_DIE_KICK;
		}
	}
}
