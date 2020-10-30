#pragma once
#include "PlayerState.h"

class PlayerFlyingState : PlayerState
{
	static PlayerState* __instance;
	bool stopIncreasingSpeed = false;

public:
	PlayerFlyingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerFlyingState();
};

