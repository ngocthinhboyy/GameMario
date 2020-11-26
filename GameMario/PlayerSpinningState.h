#pragma once
#include "PlayerState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "PlayerRunningState.h"
#include "PlayerFallingState.h"
#include "Tail.h"
#include "Grid.h"

class PlayerSpinningState : PlayerState
{
	static PlayerState* __instance;
	static LPGAMEOBJECT tailFront; 
	static LPGAMEOBJECT tailBack;

public:
	static int offset;
	PlayerSpinningState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerSpinningState();
};

