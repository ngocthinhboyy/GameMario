#include "PlayerSkiddingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerRunningState.h"
#include "debug.h"

PlayerState* PlayerSkiddingState::__instance = NULL;
PlayerSkiddingState::PlayerSkiddingState()
{
}

void PlayerSkiddingState::SetAnimation(int levelPlayer)
{
	switch (levelPlayer)
	{
	case MARIO_LEVEL_BIG: {
		animationID = MARIO_ANI_BIG_SKID;
		break;
	}
	case MARIO_LEVEL_SMALL: {
		animationID = MARIO_ANI_SMALL_SKID;
		break;
	}
	case MARIO_LEVEL_RACCOON: {
		animationID = MARIO_ANI_RACCOON_SKID;
		break;
	}
	default:
		break;
	}
}

void PlayerSkiddingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
	mario->vx += (dt * MARIO_SPEED_ACCELERATION * 2.5 * -(mario->nx));
	if ((mario->vx * mario->nx) <= 0) {
		mario->ChangeState(PlayerRunningState::GetInstance());
	}
}

PlayerState* PlayerSkiddingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerSkiddingState();
	return __instance;
}

PlayerSkiddingState::~PlayerSkiddingState()
{
}
