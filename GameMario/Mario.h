#pragma once
#include "GameObject.h"
#include "PlayerState.h"
#include "CollisionMapObject.h"
#include "GraphicsDefine.h"
#include "MarioDefine.h"
#include "Enemy.h"


class Mario : public GameObject
{
	int level = 1;
	bool untouchable = false;
	DWORD untouchable_start = 0;

	float start_x;			// initial position of Mario at scene
	float start_y;

	static Mario* __instance;
	PlayerState* playerState = NULL;
	int heart = 4;
	int point = 0;
	int coin = 0;
	int alpha = 255;
	int offset = 0;
	DWORD startHideAndUnhide = 0;

	bool isCrouching = false;
	bool isRunning = false;
	bool isOnGround = true;
	bool isSpinning = false;
	bool isFlying = false;
	bool isGrowingUp = false;
	bool isGoingAuto = false;
	bool canGoDownIntoGate = false;
	bool canGoUpIntoGate = false;
	bool isMovingIntoGate = false;
	bool isHolding = false;
	bool isGoingByCamera = false;
	bool isTransforming = false;
	bool canJump = false;

	bool isAlreadyStandingOnGateInWorldMap = false;
public:
	float speedLast = .0f;
	Mario(float x = 0.0f, float y = 0.0f);
	void Update(DWORD dt, int scaleTime);
	void UpdateInWorldMap(DWORD dt);
	virtual void Render();

	bool GetCanGoDownIntoGate() { return this->canGoDownIntoGate; };
	void SetCanGoDownIntoGate(bool canGoDownIntoGate) { this->canGoDownIntoGate = canGoDownIntoGate; };
	bool GetCanGoUpIntoGate() { return this->canGoUpIntoGate; };
	void SetCanGoUpIntoGate(bool canGoUpIntoGate) { this->canGoUpIntoGate = canGoUpIntoGate; };
	bool GetIsTransforming() { return this->isTransforming; };
	void SetIsTransforming(bool isTransforming) { this->isTransforming = isTransforming; };
	bool GetIsGoingByCamera() { return this->isGoingByCamera; };
	void SetIsGoingByCamera(bool isGoingByCamera) { this->isGoingByCamera = isGoingByCamera; };
	void SetCanJump(bool canJump) { this->canJump = canJump; };
	bool GetCanJump() { return this->canJump; };

	bool GetIsMovingIntoGate() { return this->isMovingIntoGate; };
	void SetIsMovingIntoGate(bool isMovingIntoGate) { this->isMovingIntoGate = isMovingIntoGate; };

	void SetIsGrowingUp(bool isGrowingUp) { this->isGrowingUp = isGrowingUp; };

	//void SetState(int state);
	void SetLevel(int level) { this->level = level; }
	void StartUntouchable() { untouchable = true; untouchable_start = GetTickCount64(); }
	void SetUntouchable(bool untouchable) { this->untouchable = untouchable; untouchable_start = 0; };

	bool GetUntouchable() { return this->untouchable; };

	void SetIsSpinning(bool isSpinning) { this->isSpinning = isSpinning; };
	void SetOffset(int offset) { this->offset = offset; };

	void SetIsGoingAuto(bool isGoingAuto) { this->isGoingAuto = isGoingAuto; };


	void Reset();
	void SetIsHolding(bool isHolding) { this->isHolding = isHolding; };
	bool GetIsHolding() { return this->isHolding; };
	void SetIsCrouching(bool isCrouching) { this->isCrouching = isCrouching; };
	bool GetIsCrouChing() { return this->isCrouching; };
	void SetIsRunning(bool isRunning) { this->isRunning = isRunning; };
	bool GetIsRunning() { return this->isRunning; };
	void SetIsOnGround(bool isOnGround) { this->isOnGround = isOnGround; };
	bool GetIsOnGround() { return this->isOnGround; };
	void SetIsFlying(bool isFlying) { this->isFlying = isFlying; };
	bool GetIsFlying() { return this->isFlying; };
	void SetAlpha(int alpha) { this->alpha = alpha; };
	int GetAlpha() { return this->alpha; };
	void StartHideAndUnhide() { startHideAndUnhide = GetTickCount64(); };

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	static Mario* GetInstance();

	int GetLevel() { return level; };
	void ChangeState(PlayerState* newState);
	PlayerState* GetPlayerState() { return this->playerState; };

	void SetHeart(int heart) { this->heart = heart; };
	int GetHeart() { return this->heart; };
	void SetPoint(int point) { this->point = point; };
	int GetPoint() { return this->point; };
	void SetCoin(int coin) { this->coin = coin; };
	int GetCoin() { return this->coin; };

	void CollisionWithEnemy(LPCOLLISIONEVENT collisionEvent, LPENEMY collisionMapObject);
	//void CalcPotentialCollisionsWithEnemy(vector<LPENEMY>* coEnemies, vector<LPCOLLISIONEVENT>& coEvents);

	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);

	//void CheckStandingOn

};