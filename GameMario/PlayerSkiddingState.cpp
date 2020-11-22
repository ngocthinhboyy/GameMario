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
	case MARIO_LEVEL_FIRE: {
		animationID = MARIO_ANI_FIRE_SKID;
		break;
	}
	default:
		break;
	}
}

void PlayerSkiddingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	mario->vx += (dt * marioSkiddingAcceleration * -(mario->nx));
	float check = mario->speedLast - abs(mario->vx);
	if ( check >= MARIO_RUNNING_MAX_SPEED * 0.1429 * 2 || mario->vx * mario->nx <= 0) {
		PlayerRunningState::lastStateIsSkidding = true;
		mario->ChangeState(PlayerRunningState::GetInstance());
	}
	/*if ((mario->vx * mario->nx) <= 0) {
		PlayerRunningState::lastStateIsSkidding = true;
		DebugOut(L"BBBBBBB \n");
		mario->ChangeState(PlayerRunningState::GetInstance());
	}*/
}

void PlayerSkiddingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
}

PlayerState* PlayerSkiddingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerSkiddingState();
	//Mario::GetInstance()->speedLast = abs(Mario::GetInstance()->vx);
	return __instance;
}

PlayerSkiddingState::~PlayerSkiddingState()
{
}
