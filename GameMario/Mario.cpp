#include <algorithm>
#include <assert.h>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "PlayerCrouchingState.h"
#include "PlayerStandingState.h"
#include "PlayerState.h"
#include <memory>
#include "CollisionMapObject.h"
#include "Textures.h"
#include "PlayScene.h"
#include "QuestionBrick.h"
#include "Item.h"
#include "Coin.h"
#include "StaticObjectDefine.h"
#include "ItemDefine.h"
#include "Grid.h"
#include "PlayerRunningState.h"
#include "PlayerThrowingFireballState.h"
#include "PlayerLevelDownTransformState.h"
#include "PlayerSpinningState.h"
#include "Gate.h"
#include "PlayerMovingDownAndUpState.h"
#include "EspecialBrick.h"
#include "ButtonP.h"
#include "RandomGift.h"
#include "PlayerBonusTransformState.h"
#include "PlayerDieState.h"
#include "WorldMap.h"
#include "EnemyDefine.h"
#include "MovingBrick.h"
#include "PlayerGoingAutoState.h"
#include "PlayerGoingByCameraState.h"

Mario* Mario::Mario::__instance = NULL;
Mario* Mario::GetInstance() {
	if (__instance == NULL) __instance = new Mario(0, 0);
	return __instance;
}
void Mario::ChangeState(PlayerState* newState)
{
	this->playerState = newState;
}

void Mario::CollisionWithEnemy(LPCOLLISIONEVENT collisionEvent, LPENEMY collisionMapObject)
{
	
}


//void Mario::CalcPotentialCollisionsWithEnemy(vector<LPENEMY>* coEnemies, vector<LPCOLLISIONEVENT>& coEvents)
//{
//	for (UINT i = 0; i < coEnemies->size(); i++)
//	{
//		LPCOLLISIONEVENT e = SweptAABBEx(coEnemies->at(i));
//
//		if (e->t > 0 && e->t <= 1.0f)
//			coEvents.push_back(e);
//		else
//			delete e;
//	}
//
//	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
//}

void Mario::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
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
			if (isRunning) {
				//isCollisionWithWall = true;
				PlayerRunningState::lastStateIsSkidding = true;
				vx += (dt * MARIO_SPEED_ACCELERATION * 3.5 * -nx);
			}
			else {
				vx = 0;
			}
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
			vy = 0;
			if (collisionEvent->ny < 0)
				isOnGround = true;
			else if (collisionEvent->ny > 0)
				isOnGround = false;
		}
	}
}

Mario::Mario(float x, float y) : GameObject()
{
	untouchable = 0;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	this->isCrouching = false;
}

void Mario::Update(DWORD dt, int scaleTime)
{
	playerState->Update(dt);
	vy += MARIO_GRAVITY * dt;
	GameObject::Update(dt, scaleTime);
	if (noCollisionConsideration)
		return;

	PlayScene * scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjects = scene->objects;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = false;
	}
	if (untouchable) {
		CalcPotentialCollisions(&coEnemies, coEvents);
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
		CalcPotentialCollisions(&coObjects, coEvents);
	}
	else {
		CalcPotentialCollisions(&coEnemies, coEvents);
		CalcPotentialCollisions(&coObjects, coEvents);
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
	}

	if (coEvents.size() == 0 || noCollisionConsideration)
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
			else if (Gate* gate = dynamic_cast<Gate*> (e->obj)) {
				gate->CollisionWithPlayer(e);
			}
			else if (LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
				enemy->CollisionWithPlayer(e);
			}
			else if (LPITEM item = dynamic_cast<LPITEM> (e->obj)) {
				if (item->stillAlive) {
					item->CollisionWithPlayer(e);
				}
			}
			else if (QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*> (e->obj)) {
				if (e->nx != 0) {
					PlayerRunningState::lastStateIsSkidding = true;
					vx += (dt * MARIO_SPEED_ACCELERATION * 3.5 * -this->nx);
				}
				else
					questionBrick->CollisionWithPlayer(e);
			}
			else if (EspecialBrick* especialBrick = dynamic_cast<EspecialBrick*> (e->obj)) {
				if (e->nx != 0) {
					PlayerRunningState::lastStateIsSkidding = true;
					vx += (dt * MARIO_SPEED_ACCELERATION * 3.5 * -this->nx);
				}
				else
					especialBrick->CollisionWithPlayer(e);
			}
			else if (MovingBrick* movingBrick = dynamic_cast<MovingBrick*> (e->obj)) {
				if (e->nx != 0) {
					/*PlayerRunningState::lastStateIsSkidding = true;
					vx += (dt * MARIO_SPEED_ACCELERATION * 3.5 * -this->nx);*/
					if (e->nx < 0) {
						this->vx = movingBrick->vx;
					}
				}
				else
					movingBrick->CollisionWithPlayer(e);
			}
			else if (Fireball* fireball = dynamic_cast<Fireball*> (e->obj)) {
				if (untouchable) {
					if (e->nx != 0)
						x += dx;
					if (e->ny != 0)
					{
						if (e->ny > 0)
							y -= dy;
						if (e->ny < 0)
							y += dy;
					}
				}
				else
				{
					y -= 2;
					if (e->ny > 0)
						y -= dy;
					if (isHolding) {
						vector<LPGAMEOBJECT> enemies = scene->enemies;
						for (auto enemy : enemies) {
							if (Koopa * koopa = dynamic_cast<Koopa*> (enemy)) {
								if (koopa->GetIsHold()) {
									koopa->SetIsUpsideDown(false);
									koopa->noCollisionConsideration = false;
									koopa->y -= 40;
									koopa->SetState(ENEMY_STATE_WALKING);
									koopa->SetIsHold(false);
									koopa->vx = KOOPA_WALKING_SPEED_X * this->nx;
									koopa->nx = this->nx;
									isHolding = false;
								}
							}
						}
					}
					if (level >= MARIO_LEVEL_BIG) {
						StartUntouchable();
						fireball->noCollisionConsideration = true;
						ChangeState(PlayerLevelDownTransformState::GetInstance());
						scene->StopGame(1000);
					}
					else if (level == MARIO_LEVEL_SMALL) {
						ChangeState(PlayerDieState::GetInstance());
						noCollisionConsideration = true;
						vx = 0;
						scene->StopGame(5000);
					}
				}
			}
			else if (ButtonP* buttonP = dynamic_cast<ButtonP*> (e->obj)) {
				buttonP->CollisionWithPlayer(e);
			}
			else if (RandomGift* randomGift = dynamic_cast<RandomGift*> (e->obj)) {
				randomGift->CollisionWithPlayer(e);
			}
		}
	}
	// clean up collision events
	
	if (x <= 0)
		x = 0;
	if (!isGoingAuto) {
		float maxX = Camera::GetInstance()->GetCamXEndScene() - 98;
		if (x >= maxX) {
			x = maxX;
			vx = 0;
		}
	}
	else {
		float maxXAutoGo = Camera::GetInstance()->GetCamXEndScene() + 60;
		if (x > maxXAutoGo)
			vx = 0;
	}
	float camx, camy;
	Camera::GetInstance()->GetCamPos(camx, camy);
	if (x < camx && !isGoingByCamera) {
		isGoingByCamera = true;
		ChangeState(PlayerGoingByCameraState::GetInstance());
	}
	if (y > camy + 580) {
		level = MARIO_LEVEL_BIG;
		ChangeState(PlayerDieState::GetInstance());

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Mario::UpdateInWorldMap(DWORD dt)
{

	WorldMap* worldMap = dynamic_cast<WorldMap*> (Game::GetInstance()->GetCurrentScene());
	vector<GateInWorldMap*> gatesInWorldMap = worldMap->gatesInWorldMap;
	GateInWorldMap* gateStandingOn = NULL;
	bool alreadyHasGate = false;

	for (auto gate : gatesInWorldMap) {
		if (gate->CheckMarioIsStandingOn() != -1 && !alreadyHasGate) {
			gateStandingOn = gate;
			alreadyHasGate = true;
			if (gateStandingOn->gameObjectID != PlayerInWorldMapState::idGateMarioStandingOn)
				isAlreadyStandingOnGateInWorldMap = false;
		}
	}
	if (gateStandingOn != NULL && !isAlreadyStandingOnGateInWorldMap) {
		PlayerInWorldMapState::directionX = gateStandingOn->GetDirectionX();
		PlayerInWorldMapState::directionY = gateStandingOn->GetDirectionY();
		PlayerInWorldMapState::idGateMarioStandingOn = gateStandingOn->gameObjectID;
		x = gateStandingOn->x;
		y = gateStandingOn->y;
		vx = 0;
		vy = 0;
		isAlreadyStandingOnGateInWorldMap = true;
	}
	else if (gateStandingOn == NULL && isAlreadyStandingOnGateInWorldMap) {
		isAlreadyStandingOnGateInWorldMap = false;
	}
	playerState->Update(dt);
	dx = vx * dt;
	dy = vy * dt;
	x += dx;
	y += dy;
}

void Mario::Render()
{
	if (untouchable) {
		DWORD now = GetTickCount64();
		if (startHideAndUnhide != 0 && now - startHideAndUnhide >= 60) {
			alpha = 0;
			startHideAndUnhide = now;
		}
		else {
			alpha = 170;
		}
	}
	else {
		startHideAndUnhide = 0;
		alpha = 255;
	}
	D3DXVECTOR2 scale;
	LPANIMATION ani = playerState->GetAnimation();
	if (level == MARIO_LEVEL_RACCOON && nx > 0 && !isSpinning)
		offset = 6;
	else if (level == MARIO_LEVEL_RACCOON && isSpinning) {
		offset = PlayerSpinningState::offset;
	}
	else {
		offset = 0;
	}
	if (isGrowingUp) {
		offset = 0;
		if (nx < 1)
			scale = D3DXVECTOR2(-1, 1);
		else
			scale = D3DXVECTOR2(1, 1);
	}
	else {
		if (nx < 1)
			scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
		else
			scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	}
	if (ani != NULL) {
		if (isCrouching)
			ani->Render(x, y - 5, alpha, scale, offset);
		else
			ani->Render(x, y, alpha, scale, offset);
	}
	//RenderBoundingBox();
}

//void Mario::SetState(int state)
//{
//}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
	{
		if (isCrouching) {
			left = x;
			top = y;
			right = left + MARIO_BIG_CROUCH_BBOX_WIDTH;
			bottom = top + MARIO_BIG_CROUCH_BBOX_HEIGHT;
		}
		else
		{
			left = x;
			top = y;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}

	}
	else if (level == MARIO_LEVEL_RACCOON) {

		if (isCrouching) {
			left = x;
			top = y;
			right = left + MARIO_BIG_CROUCH_BBOX_WIDTH;
			bottom = top + MARIO_BIG_CROUCH_BBOX_HEIGHT;
		}
		else
		{
			left = x;
			top = y;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
	else if(level == MARIO_LEVEL_SMALL)
	{
		if (isGrowingUp) {
			left = x;
			top = y;
			if (PlayerBonusTransformState::stateWhenGrowingUp == STATE_MARIO_SMALL) {
				right = left + WIDTH_STATE_MARIO_SMALL_GROW_UP;
				bottom = top + HEIGHT_STATE_MARIO_SMALL_GROW_UP;
			}
			else if (PlayerBonusTransformState::stateWhenGrowingUp == STATE_MARIO_MIDDLE) {
				right = left + WIDTH_STATE_MARIO_MIDDLE_GROW_UP;
				bottom = top + HEIGHT_STATE_MARIO_MIDDLE_GROW_UP;
			}
			else if (PlayerBonusTransformState::stateWhenGrowingUp == STATE_MARIO_BIG) {
				right = left + WIDTH_STATE_MARIO_BIG_GROW_UP;
				bottom = top + HEIGHT_STATE_MARIO_BIG_GROW_UP;
			}
		}
		else {
			left = x;
			top = y;
			right = left + MARIO_SMALL_BBOX_WIDTH;
			bottom = top + MARIO_SMALL_BBOX_HEIGHT;
		}
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	isCrouching = false;
	isRunning = false;
	isOnGround = true;
	isSpinning = false;
	isFlying = false;
	isGrowingUp = false;
	isGoingAuto = false;
	isHolding = false;
	canGoDownIntoGate = false;
	canGoUpIntoGate = false;
	isMovingIntoGate = false;
	ChangeState(PlayerStandingState::GetInstance());
}

