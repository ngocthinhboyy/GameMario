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
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	static Mario* __instance;
	PlayerState* playerState;
	bool isCrouching = false;
	bool isRunning = false;
	bool isOnGround = true;
public:
	Mario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int level) { this->level = level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }


	void Reset();
	void SetIsCrouching(bool isCrouching) { this->isCrouching = isCrouching; };
	bool GetIsCrouChing() { return this->isCrouching; };
	void SetIsRunning(bool isRunning) { this->isRunning = isRunning; };
	bool GetIsRunning() { return this->isRunning; };
	void SetIsOnGround(bool isOnGround) { this->isOnGround = isOnGround; };
	bool GetIsOnGround() { return this->isOnGround; };

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	static Mario* GetInstance();

	int GetLevel() { return level; };
	void ChangeState(PlayerState* newState);
	PlayerState* GetPlayerState() { return this->playerState; };

	void CollisionWithEnemy(LPCOLLISIONEVENT collisionEvent, LPENEMY collisionMapObject);
	//void CalcPotentialCollisionsWithEnemy(vector<LPENEMY>* coEnemies, vector<LPCOLLISIONEVENT>& coEvents);

	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);

};