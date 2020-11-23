#pragma once
#include "PlayerState.h"

class PlayerFallingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerFallingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerFallingState();
};

