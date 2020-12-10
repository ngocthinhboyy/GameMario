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
	Mario* mario = Mario::GetInstance();
	if (GetTickCount64() - startTimeDie <= 600) {
		mario->vy = 0;
	}
	else if (GetTickCount64() - startTimeDie <= 1000) {
		mario->vy = -0.35f;
	}
	else {
		mario->vy = 0.55f;
	}
	mario->dy = mario->vy * dt;
	mario->y += mario->dy;
	mario->noCollisionConsideration = true;
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
