#pragma once
#include "PlayerState.h"

class PlayerHighJumpingState : PlayerState
{
	static PlayerState* __instance;
	bool stopIncreasingSpeed = false;
public:
	PlayerHighJumpingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerHighJumpingState();
};