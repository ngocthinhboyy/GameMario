#include "PlayerMovingDownAndUpState.h"
#include "Mario.h"
#include "game.h"
#include "debug.h"
#include "PlayerStandingState.h"

PlayerState* PlayerMovingDownAndUpState::__instance = NULL;
PlayerMovingDownAndUpState::PlayerMovingDownAndUpState()
{
}

void PlayerMovingDownAndUpState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_GO_DOWN_UP;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_GO_DOWN_UP;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_GO_DOWN_UP;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_GO_DOWN_UP;
		break;
	}
	default:
		break;
	}
}

void PlayerMovingDownAndUpState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->GetCanGoDownIntoGate()) {
		mario->vy = 0;
	}
	else if (mario->GetCanGoUpIntoGate()) {
		mario->vy = -0.09f;
	}
}

void PlayerMovingDownAndUpState::KeyState(BYTE* states)
{
}

PlayerState* PlayerMovingDownAndUpState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerMovingDownAndUpState();
	SetAnimation();
	Mario::GetInstance()->SetIsMovingIntoGate(true);
	return __instance;
}

PlayerMovingDownAndUpState::~PlayerMovingDownAndUpState()
{
}
