#pragma once
#include "PlayerState.h"


class PlayerStandingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerStandingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerStandingState();
};

