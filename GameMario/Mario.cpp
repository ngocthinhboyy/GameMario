#include <algorithm>
#include <assert.h>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "PlayerCrouchingState.h"
#include "PlayerStandingState.h"
#include "PlayerState.h"
#include <memory>
#include "CollisionMapObject.h"
#include "Textures.h"

Mario* Mario::Mario::__instance = NULL;
Mario* Mario::GetInstance() {
	if (__instance == NULL) __instance = new Mario(0, 0);
	return __instance;
}
void Mario::ChangeState(PlayerState* newState)
{
	this->playerState = newState;
}


void Mario::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, CollisionMapObject* collisionMapObject)
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
			vx = 0;
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

Mario::Mario(float x, float y) : GameObject()
{
	//level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	//SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	this->isCrouching = false;
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	playerState->Update(dt);

	vy += MARIO_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (x <= 0)
			x = 0;
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
			if (CollisionMapObject* collMapObj = dynamic_cast<CollisionMapObject*> (e->obj)) {
				CollisionWithCollisionMapObject(e, collMapObj);
			}
			else {
				if (e->nx != 0) vx = 0;
				if (e->ny != 0) vy = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Mario::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	D3DXVECTOR2 scale;
	LPANIMATION ani = playerState->GetAnimation();
	if (nx < 1)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if(ani != NULL)
		ani->Render(x, y, alpha, scale);

	RenderBoundingBox();
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
	{

		if (isCrouching) {
			left = x - MARIO_BIG_CROUCH_BBOX_WIDTH/2;
			top = y - MARIO_BIG_CROUCH_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_CROUCH_BBOX_WIDTH;
			bottom = top + MARIO_BIG_CROUCH_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}

	}
	else if (level == MARIO_LEVEL_RACCOON) {

		if (isCrouching) {
			left = x - MARIO_BIG_CROUCH_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_CROUCH_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_CROUCH_BBOX_WIDTH;
			bottom = top + MARIO_BIG_CROUCH_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
	else if(level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	SetPosition(x, 0);
	SetSpeed(0, 0);
	ChangeState(PlayerStandingState::GetInstance());
}

