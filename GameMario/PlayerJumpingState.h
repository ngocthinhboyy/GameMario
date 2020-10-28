#pragma once
#include "PlayerState.h"

class PlayerJumpingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerJumpingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerJumpingState();
};

