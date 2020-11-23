#pragma once
#include "PlayerState.h"
class PlayerTransformState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
public:
	PlayerTransformState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerTransformState();
};
