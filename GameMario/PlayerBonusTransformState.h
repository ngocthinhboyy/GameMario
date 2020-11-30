#pragma once
#include "PlayerState.h"
class PlayerBonusTransformState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
	static DWORD startTransform;
public:
	PlayerBonusTransformState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerBonusTransformState();
};
