#pragma once
#include "PlayerState.h"
class PlayerFallingSlowlyState : PlayerState
{
	static PlayerState* __instance;
	bool isSlowly = false;
	DWORD timePress = 0;
public:
	PlayerFallingSlowlyState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerFallingSlowlyState();
};

