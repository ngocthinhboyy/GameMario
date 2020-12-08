#pragma once
#include "PlayerState.h"
class PlayerGoingAutoState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerGoingAutoState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerGoingAutoState();
};

