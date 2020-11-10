#pragma once
#include "PlayerState.h"
class PlayerSkiddingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerSkiddingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerSkiddingState();
};


