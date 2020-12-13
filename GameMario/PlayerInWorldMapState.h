#pragma once
#include "PlayerState.h"
class PlayerInWorldMapState : PlayerState
{
	static PlayerState* __instance;
public:
	PlayerInWorldMapState();
	static void SetAnimation();
	static int directionX;
	static int directionY;
	static int idGateMarioStandingOn;
	void Update(int dt);
	void OnKeyDown(int KeyCode);
	void KeyState(BYTE* states);
	static PlayerState* GetInstance();
	~PlayerInWorldMapState();
};

