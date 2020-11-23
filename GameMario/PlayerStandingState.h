#pragma once
#include "PlayerState.h"


class PlayerStandingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerStandingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerStandingState();
};

