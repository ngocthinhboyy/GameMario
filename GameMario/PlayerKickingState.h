#pragma once
#include "PlayerState.h"

class PlayerKickingState : PlayerState
{
	static PlayerState* __instance;
	static DWORD timeStartKicking;
public:
	PlayerKickingState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerKickingState();
};

