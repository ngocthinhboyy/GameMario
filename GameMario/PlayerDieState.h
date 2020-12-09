#pragma once
#include "PlayerState.h"
class PlayerDieState : PlayerState
{
	static PlayerState* __instance;
	static DWORD startTimeDie;
public:
	PlayerDieState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerDieState();
};

