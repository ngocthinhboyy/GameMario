#pragma once
#include "GameObject.h"
#include "PlayerState.h"
#include "CollisionMapObject.h"
#include "GraphicsDefine.h"
#include "MarioDefine.h"
#include "Enemy.h"


class Mario : public GameObject
{
	int level;
	bool untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	static Mario* __instance;
	PlayerState* playerState;
	bool isCrouching = false;
	bool isRunning = false;
	bool isOnGround = true;
	bool isSpinning = false;
	int heart = 4;
	int point = 0;
	int coin = 0;
	int alpha = 255;
	int offset = 0;
	DWORD startHideAndUnhide = 0;
public:
	float speedLast;
	Mario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt);
	virtual void Render();

	//void SetState(int state);
	void SetLevel(int level) { this->level = level; }
	void StartUntouchable() { untouchable = true; untouchable_start = GetTickCount64(); }

	bool GetUntouchable() { return this->untouchable; };

	void SetIsSpinning(bool isSpinning) { this->isSpinning = isSpinning; };
	void SetOffset(int offset) { this->offset = offset; };


	void Reset();
	void SetIsCrouching(bool isCrouching) { this->isCrouching = isCrouching; };
	bool GetIsCrouChing() { return this->isCrouching; };
	void SetIsRunning(bool isRunning) { this->isRunning = isRunning; };
	bool GetIsRunning() { return this->isRunning; };
	void SetIsOnGround(bool isOnGround) { this->isOnGround = isOnGround; };
	bool GetIsOnGround() { return this->isOnGround; };
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

};