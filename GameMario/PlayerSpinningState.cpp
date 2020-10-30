#include "PlayerSpinningState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "PlayerRunningState.h"

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

void PlayerSpinningState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	bool isLastFrame = animation->GetIsLastFrame();
	/*if (game->IsKeyDown(DIK_RIGHT))
	{
		if (abs(mario->vx) == 0) {
			mario->vx = MARIO_WALKING_SPEED;
		}
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (abs(mario->vx) == 0) {
			mario->vx = -MARIO_WALKING_SPEED;
		}
		mario->nx = -1;
	}*/
	if (isLastFrame) {
		animation->ResetAnimation();
		if(game->IsKeyDown(DIK_A))
			mario->ChangeState(PlayerRunningState::GetInstance());
		else
			mario->ChangeState(PlayerStandingState::GetInstance());
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
