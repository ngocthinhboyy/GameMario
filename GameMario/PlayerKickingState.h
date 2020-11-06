#pragma once
#include "PlayerState.h"

class PlayerKickingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerKickingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerKickingState();
};

