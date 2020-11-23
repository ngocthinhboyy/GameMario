#pragma once
#include "PlayerState.h"

class PlayerCrouchingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerCrouchingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerCrouchingState();
};

