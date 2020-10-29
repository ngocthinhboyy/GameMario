#include "PlayerSpinningState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"

PlayerState* PlayerSpinningState::__instance = NULL;
PlayerSpinningState::PlayerSpinningState()
{
}

void PlayerSpinningState::SetAnimation(int levelPlayer)
{
	switch (levelPlayer)
	{
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_SPIN;
	}
	default:
		break;
	}
}

void PlayerSpinningState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
}

void PlayerSpinningState::OnKeyUp(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_A:
		mario->ChangeState(PlayerStandingState::GetInstance());
		break;
	default:
		break;
	}
}

PlayerState* PlayerSpinningState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerSpinningState();
	return __instance;
}

PlayerSpinningState::~PlayerSpinningState()
{
}
