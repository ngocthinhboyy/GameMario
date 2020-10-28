#pragma once
#include "PlayerState.h"

class PlayerCrouchingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerCrouchingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerCrouchingState();
};

