#pragma once
#include "PlayerState.h"

class PlayerWalkingState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
public:
	PlayerWalkingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerWalkingState();
};

