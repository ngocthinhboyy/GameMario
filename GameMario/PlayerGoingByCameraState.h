#pragma once
#include "PlayerState.h"

class PlayerGoingByCameraState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerGoingByCameraState();
	static void SetAnimation();
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerGoingByCameraState();
};

