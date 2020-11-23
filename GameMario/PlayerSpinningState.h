#pragma once
#include "PlayerState.h"

class PlayerSpinningState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerSpinningState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerSpinningState();
};

