#pragma once
#include "PlayerState.h"

class PlayerSpinningState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerSpinningState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerSpinningState();
};

