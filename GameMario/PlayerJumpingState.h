#pragma once
#include "PlayerState.h"

class PlayerJumpingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerJumpingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerJumpingState();
};

