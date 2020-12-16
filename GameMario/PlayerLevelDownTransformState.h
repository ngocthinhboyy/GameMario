#pragma once
#include "PlayerState.h"
class PlayerLevelDownTransformState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
	static DWORD startTransform;
	static DWORD timeStartTransform;
	int countTransform = 0;
	bool changeCrouching = false;
public:
	PlayerLevelDownTransformState();
	static void SetAnimation();
	void Update(int dt);
	static int stateWhenGrowingUp;
	void KeyState(BYTE* states);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerLevelDownTransformState();
};
