#pragma once
#include "PlayerState.h"
class PlayerSkiddingState : PlayerState
{
	static PlayerState* __instance;
	static DWORD timeStartSkidding;
	float marioSkiddingAcceleration = MARIO_SPEED_ACCELERATION * 3.5;
public:
	PlayerSkiddingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerSkiddingState();
};


