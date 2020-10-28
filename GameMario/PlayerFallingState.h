#pragma once
#include "PlayerState.h"

class PlayerFallingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerFallingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerFallingState();
};

