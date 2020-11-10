#pragma once
#include "PlayerState.h"
class PlayerThrowingFireballState : PlayerState
{
	static PlayerState* __instance;
	static int quantityFireball;
	DWORD lastThrowingTime;
public:
	PlayerThrowingFireballState();
	void SetAnimation(int levelPlayer);
	void Update(int dt);
	static void decreaseQuantityOneValue() { quantityFireball--; };
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerThrowingFireballState();
};

