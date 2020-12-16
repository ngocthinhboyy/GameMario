#pragma once
#include "PlayerState.h"
class PlayerBonusTransformState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
	static DWORD startTransform;
	static DWORD timeStartTransform;
	int countTransform = 0;
public:
	PlayerBonusTransformState();
	static int stateWhenGrowingUp;
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerBonusTransformState();
};
