#pragma once
#include "PlayerState.h"

class PlayerWalkingState : PlayerState
{
	static PlayerState* __instance;
	bool isSlow = false;
public:
	PlayerWalkingState();
	static void SetAnimation(); 
	static bool prevKeyIsRight;
	static bool prevKeyIsLeft;
	void Update(int dt);
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	static PlayerState* GetInstance();
	~PlayerWalkingState();
};

