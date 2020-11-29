#pragma once
#include "PlayerState.h"
class PlayerMovingDownAndUpState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerMovingDownAndUpState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerMovingDownAndUpState();
};

