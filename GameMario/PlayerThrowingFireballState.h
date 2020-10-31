#pragma once
#include "PlayerState.h"
class PlayerThrowingFireballState : PlayerState
{
static PlayerState* __instance;
public:
	PlayerThrowingFireballState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerThrowingFireballState();
};

