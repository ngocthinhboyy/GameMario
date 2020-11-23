#pragma once
#include "PlayerState.h"
class PlayerThrowingFireballState : PlayerState
{
	static PlayerState* __instance;
	static int quantityFireball;
	DWORD lastThrowingTime;
public:
	PlayerThrowingFireballState();
	static void SetAnimation();
	void Update(int dt);
	static void decreaseQuantityOneValue() { quantityFireball--; };
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerThrowingFireballState();
};

