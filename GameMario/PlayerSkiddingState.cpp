#include "PlayerSkiddingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerRunningState.h"
#include "debug.h"
#include "AnimationDatabase.h"
#include "PlayerWalkingState.h"

PlayerState* PlayerSkiddingState::__instance = NULL;
DWORD PlayerSkiddingState::timeStartSkidding = 0;
PlayerSkiddingState::PlayerSkiddingState()
{
}

void PlayerSkiddingState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel())
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
	if (mario->GetIsRunning()) {
		if (check >= MARIO_RUNNING_MAX_SPEED * 0.1429 * 2 || mario->vx * mario->nx <= 0) {
			PlayerRunningState::lastStateIsSkidding = true;
			mario->ChangeState(PlayerRunningState::GetInstance());
		}
	}
	else {
		Game* game = Game::GetInstance();
		if (GetTickCount64() - timeStartSkidding >= 150) {
			mario->nx = -mario->nx;
			mario->ChangeState(PlayerWalkingState::GetInstance());
		}
	}
	/*if ((mario->vx * mario->nx) <= 0) {
		PlayerRunningState::lastStateIsSkidding = true;
		DebugOut(L"BBBBBBB \n");
		mario->ChangeState(PlayerRunningState::GetInstance());
	}*/
}

void PlayerSkiddingState::KeyState(BYTE* states)
{
}

PlayerState* PlayerSkiddingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerSkiddingState();
	//Mario::GetInstance()->speedLast = abs(Mario::GetInstance()->vx);
	Mario* mario = Mario::GetInstance();
	AnimationDatabase::GetInstance()->Get(animationID)->ResetAnimation();
	SetAnimation();
	timeStartSkidding =  GetTickCount64();
	if (PlayerRunningState::isCrouching)
		mario->y -= MARIO_DEVIATION_CROUCHING_Y;
	return __instance;
}

PlayerSkiddingState::~PlayerSkiddingState()
{
}
