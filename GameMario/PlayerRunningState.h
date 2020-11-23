#pragma once
#include "PlayerState.h"


class PlayerRunningState : PlayerState
{
	static PlayerState* __instance;
	static bool isMaxSpeed;
	bool increaseSpeed = true;
	bool prevKeyIsRight = false;
	bool prevKeyIsLeft = false;
	bool isSkidding = false;
public:
	PlayerRunningState();
	static bool isCrouching;
	static bool lastStateIsSkidding;
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerRunningState();
};

