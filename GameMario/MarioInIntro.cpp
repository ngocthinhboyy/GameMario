#include "MarioInIntro.h"
#include "AnimationDatabase.h"
#include "IntroMapDefine.h"
#include "MarioDefine.h"
#include "IntroMap.h"
#include "game.h"
#include "debug.h"
#include "Platform.h"
#include "DynamicObjectInIntro.h"

MarioInIntro::MarioInIntro(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->state = MARIO_IN_INTRO_STATE_STANDING;
	if (type == TYPE_RED_MARIO) {
		nx = -1;
		SetState(MARIO_IN_INTRO_STATE_STANDING);
	}
	else if (type == TYPE_GREEN_MARIO) {
		nx = 1;
		SetState(MARIO_IN_INTRO_STATE_STANDING);
	}
	timeStartMoving = GetTickCount64();
}

void MarioInIntro::Update(DWORD dt, int scaleTime)
{
	ActionInIntro();
	if(type != TYPE_RED_MARIO || state != MARIO_IN_INTRO_STATE_RACCOON_FALLING)
		vy += MARIO_GRAVITY * dt;
	GameObject::Update(dt, scaleTime);

	IntroMap* scene = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coObjects = scene->objects;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&coObjects, coEvents);
	if (coEvents.size() == 0)
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
			else if (Platform* platform = dynamic_cast<Platform*> (e->obj)) {
				if (e->ny > 0) {
					platform->vy = 0.2f;
				}
			}
			else if (DynamicObjectInIntro * object = dynamic_cast<DynamicObjectInIntro*> (e->obj)) {
				object->CollisionWithMario(e, object);
			}

		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

	if (type == TYPE_RED_MARIO) {
		if (x <= Game::GetInstance()->GetScreenWidth() / 2 && !isWalking) {
			isWalking = true;
			SetState(MARIO_IN_INTRO_STATE_STANDING);
		}
		if (timeJumping != 0) {
			if (GetTickCount64() - timeJumping >= 2700) {
				SetState(MARIO_IN_INTRO_STATE_JUMPING);
				timeJumping = 0;
			}
		}
	}

}

void MarioInIntro::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	SetAnimation();
	if(nx < 0)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else if(nx > 0)
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void MarioInIntro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + w;
	bottom = y + h;
}

void MarioInIntro::SetAnimation()
{
	if (type == TYPE_RED_MARIO) {
		switch (state)
		{
			case MARIO_IN_INTRO_STATE_STANDING:{
					animation = AnimationDatabase::GetInstance()->Get(MARIO_RED_IN_INTRO_STANDING_ANI);
					break;
				}
			case MARIO_IN_INTRO_STATE_WALKING: {
				animation = AnimationDatabase::GetInstance()->Get(MARIO_RED_IN_INTRO_WALKING_ANI);
				break;
			}
			case MARIO_IN_INTRO_STATE_JUMPING: {
				animation = AnimationDatabase::GetInstance()->Get(MARIO_RED_IN_INTRO_JUMPING_ANI);
				break;
			}
			case MARIO_IN_INTRO_STATE_CRACKING_NECK: {
				animation = AnimationDatabase::GetInstance()->Get(MARIO_RED_IN_INTRO_CRACKING_NECK_ANI);
				break;
			}
			case MARIO_IN_INTRO_STATE_RACCOON_FALLING: {
				animation = AnimationDatabase::GetInstance()->Get(MARIO_ANI_RACCOON_FALLING_SLOWLY);
				break;
			}
			case MARIO_IN_INTRO_STATE_RACCOON_STANDING: {
				animation = AnimationDatabase::GetInstance()->Get(MARIO_ANI_RACCOON_IDLE_RIGHT);
				break;
			}
			case MARIO_IN_INTRO_STATE_RACCOON_WALKING: {
				animation = AnimationDatabase::GetInstance()->Get(MARIO_ANI_RACCOON_WALKING_RIGHT);
				break;
			}
			default:
				break;
		}
	}
	else if (type == TYPE_GREEN_MARIO) {
		switch (state)
		{
		case MARIO_IN_INTRO_STATE_STANDING: {
			animation = AnimationDatabase::GetInstance()->Get(MARIO_GREEN_IN_INTRO_STANDING_ANI);
			break;
		}
		case MARIO_IN_INTRO_STATE_WALKING: {
			animation = AnimationDatabase::GetInstance()->Get(MARIO_GREEN_IN_INTRO_WALKING_ANI);
			break;
		}
		case MARIO_IN_INTRO_STATE_JUMPING: {
			animation = AnimationDatabase::GetInstance()->Get(MARIO_GREEN_IN_INTRO_JUMPING_ANI);
			break;
		}
		case MARIO_IN_INTRO_STATE_FALLING: {
			animation = AnimationDatabase::GetInstance()->Get(MARIO_GREEN_IN_INTRO_FALLING_ANI);
			break;
		}
		case MARIO_IN_INTRO_STATE_FALLING_JUMP: {
			animation = AnimationDatabase::GetInstance()->Get(MARIO_GREEN_IN_INTRO_FALLING_ANI);
			break;
		}
		default:
			break;
		}
	}
}

void MarioInIntro::SetState(int state)
{
	this->state = state;
	switch (this->state)
	{
		case MARIO_IN_INTRO_STATE_STANDING: {
			vx = 0;
			break;
		}
		case MARIO_IN_INTRO_STATE_WALKING: {
			vx = 0.3f * nx;
			break;
		}
		case MARIO_IN_INTRO_STATE_JUMPING: {
			if (type == TYPE_GREEN_MARIO) {
				vx = 0.2f * nx;
				vy = -0.65f;
			}
			else if (type == TYPE_RED_MARIO) {
				vy = -1.5f;
			}
			break;
		}
		case MARIO_IN_INTRO_STATE_HIGH_JUMPING: {
			vx = 0.2f * nx;
			vy = -1.5f;
			break;
		}
		case MARIO_IN_INTRO_STATE_RACCOON_FALLING: {
			vy = 0.1f;
			vx = -0.2f;
			break;
		}
		case MARIO_IN_INTRO_STATE_RACCOON_WALKING: {
			nx = 1;
			vx = 0.2f;
			break;
		}
										   
		default:
			break;
	}
}

void MarioInIntro::ActionInIntro()
{
	DWORD now = GetTickCount64();
	if (type == TYPE_GREEN_MARIO) {
		if ((state == MARIO_IN_INTRO_STATE_FALLING && vy == 0) || isWalking) {
			isWalking = true;
			SetState(MARIO_IN_INTRO_STATE_WALKING);
		}
		else if (state == MARIO_IN_INTRO_STATE_HIGH_JUMPING) {
			SetState(MARIO_IN_INTRO_STATE_FALLING);
		}
		else if (state == MARIO_IN_INTRO_STATE_FALLING_JUMP && vy == 0) {
			SetState(MARIO_IN_INTRO_STATE_HIGH_JUMPING);
		}
		else if (now - timeStartMoving >= 620 && state != MARIO_IN_INTRO_STATE_FALLING ) {
			SetState(MARIO_IN_INTRO_STATE_FALLING_JUMP);
		}
		else if (now - timeStartMoving >= 600 && state != MARIO_IN_INTRO_STATE_FALLING) {
			SetState(MARIO_IN_INTRO_STATE_JUMPING);
		}
		else if (now - timeStartMoving >= 500 && state != MARIO_IN_INTRO_STATE_FALLING) {
			SetState(MARIO_IN_INTRO_STATE_WALKING);
		}
	}
	else if (type == TYPE_RED_MARIO) {
		if (now - timeStartMoving >= 500 && !isWalking) {
			SetState(MARIO_IN_INTRO_STATE_WALKING);
		}
	}
}

void MarioInIntro::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
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
		else {
			if (state == MARIO_IN_INTRO_STATE_RACCOON_FALLING) {
				SetState(MARIO_IN_INTRO_STATE_RACCOON_WALKING);
			}
			vy = 0;
		}
	}
}

MarioInIntro::~MarioInIntro()
{
}
