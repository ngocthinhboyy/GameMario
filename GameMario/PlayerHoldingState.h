#pragma once

#include "PlayerState.h"

class PlayerHoldingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerHoldingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerHoldingState();
};

