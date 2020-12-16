#include "PlayerKickingState.h"
#include "Mario.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "Debug.h"

PlayerState* PlayerKickingState::__instance = NULL;
DWORD PlayerKickingState::timeStartKicking = 0;
PlayerKickingState::PlayerKickingState()
{
}

void PlayerKickingState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()){
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_KICK;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_KICK;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_KICK;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_KICK;
		break;
	}
	default:
		break;
	}
}

void PlayerKickingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
}

void PlayerKickingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (GetTickCount64() - timeStartKicking >= 140) {
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

PlayerState* PlayerKickingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerKickingState();
	AnimationDatabase::GetInstance()->Get(animationID)->ResetAnimation();
	SetAnimation();
	timeStartKicking = GetTickCount64();
	return __instance;
}

PlayerKickingState::~PlayerKickingState()
{
}
