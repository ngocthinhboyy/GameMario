#pragma once
#include "PlayerState.h"
class PlayerSkiddingState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerSkiddingState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	static PlayerState* GetInstance();
	~PlayerSkiddingState();
};


