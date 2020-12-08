#include "PlayerGoingAutoState.h"
#include "Mario.h"
PlayerState* PlayerGoingAutoState::__instance = NULL;
PlayerGoingAutoState::PlayerGoingAutoState()
{
}

void PlayerGoingAutoState::SetAnimation()
{
	switch (Mario::GetInstance()->GetLevel()) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_WALKING_RIGHT;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_WALKING_RIGHT;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_WALKING_RIGHT;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_WALKING_RIGHT;
		break;
	}
	default:
		break;
	}
}

void PlayerGoingAutoState::Update(int dt)
{
	if (Mario::GetInstance()->GetIsOnGround() && Mario::GetInstance()->x <= 8448) {
		Mario::GetInstance()->vx = MARIO_WALKING_SPEED * 2;
	}
}

void PlayerGoingAutoState::KeyState(BYTE* states)
{
}

PlayerState* PlayerGoingAutoState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerGoingAutoState();
	Mario::GetInstance()->nx = 1;
	Mario::GetInstance()->SetIsGoingAuto(true);
	SetAnimation();
	return __instance;
}

PlayerGoingAutoState::~PlayerGoingAutoState()
{
}
