#pragma once
#include "PlayerState.h"
class PlayerLevelDownTransformState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
public:
	PlayerLevelDownTransformState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerLevelDownTransformState();
};
