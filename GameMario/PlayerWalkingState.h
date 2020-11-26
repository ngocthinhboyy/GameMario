#pragma once
#include "PlayerState.h"

class PlayerWalkingState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
public:
	PlayerWalkingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerWalkingState();
};

