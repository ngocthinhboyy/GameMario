#pragma once
#include "PlayerState.h"


class PlayerRunningState : PlayerState
{
	static PlayerState* __instance;
	bool isMaxSpeed = false;
	bool increaseSpeed = true;
	bool prevKeyIsRight = false;
	bool prevKeyIsLeft = false;
	bool isSkidding = false;
public:
	PlayerRunningState();
	static bool lastStateIsSkidding;
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerRunningState();
};

