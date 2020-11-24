#pragma once
#include "PlayerState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "Fireball.h"
#include "PlayScene.h"
#include "DynamicObjectDefine.h"
#include "Grid.h"
#include "debug.h"
class PlayerThrowingFireballState : PlayerState
{
	static PlayerState* __instance;
	DWORD lastThrowingTime;
public:
	PlayerThrowingFireballState();
	static void SetAnimation();
	static int quantityFireball;
	void Update(int dt);
	static void decreaseQuantityOneValue() {  quantityFireball--; };
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerThrowingFireballState();
};

