#pragma once

#include "PlayerState.h"

class PlayerHoldingState : PlayerState
{
	static PlayerState* __instance;
	bool isMaxSpeed = false;
	bool increaseSpeed = true;
	bool prevKeyIsRight = false;
	bool prevKeyIsLeft = false;
	bool isTurnBack = false;
	static bool isStanding;
	static bool stopIncreasingSpeed;
	static bool alreadyMaxJumping;
	static bool isJumping;
public:
	PlayerHoldingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerHoldingState();
};

