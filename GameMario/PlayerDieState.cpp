#include "PlayerDieState.h"
#include "Mario.h"

PlayerDieState::PlayerDieState()
{
}

void PlayerDieState::SetAnimation()
{
	switch (Mario::GetInstance()->GetLevel()) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_DIE;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_DIE;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_DIE;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_DIE;
		break;
	}
	default:
		break;
	}
}

void PlayerDieState::Update(int dt)
{
	if (GetTickCount64() - startTimeDie <= 200) {
		Mario::GetInstance()->vy = -0.5f;
	}
}

void PlayerDieState::KeyState(BYTE* states)
{

}

PlayerState* PlayerDieState::__instance = NULL;
DWORD PlayerDieState::startTimeDie = 0;
PlayerState* PlayerDieState::GetInstance()
{
    if (__instance == NULL) __instance = new PlayerDieState();
	SetAnimation();
	startTimeDie = GetTickCount64();
    return __instance;
}

PlayerDieState::~PlayerDieState()
{
}
