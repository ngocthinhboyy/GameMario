#include "PlayerFallingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "PlayerJumpingState.h"
#include "debug.h"
#include "PlayerCrouchingState.h"
#include "PlayerRunningState.h"


PlayerFallingState::PlayerFallingState()
{
}

PlayerState* PlayerFallingState::__instance = NULL;

void PlayerFallingState::SetAnimation(int levelPlayer)
{
	Mario* mario = Mario::GetInstance();
	switch (levelPlayer)
	{
	case MARIO_LEVEL_BIG: {
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_BIG_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_BIG_FALL;
			else
				animationID = MARIO_ANI_BIG_FALL;
		}
		break;
	}
	case MARIO_LEVEL_SMALL:
		if (mario->nx == 1)
			animationID = MARIO_ANI_SMALL_FALL;
		else
			animationID = MARIO_ANI_SMALL_FALL;
		break;
	case MARIO_LEVEL_RACCOON:
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_RACCOON_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_RACCOON_FALL;
			else
				animationID = MARIO_ANI_RACCOON_FALL;
		}
		break;
	default:
		break;
	}
}

void PlayerFallingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
	if (mario->vy == 0) {
		if (mario->GetIsCrouChing() && mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->ChangeState(PlayerCrouchingState::GetInstance());
		else if (abs(mario->vx) > MARIO_WALKING_SPEED)
		{
			mario->ChangeState(PlayerRunningState::GetInstance());
		}
		else {
			mario->ChangeState(PlayerStandingState::GetInstance());
		}
	}
}
void PlayerFallingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_DOWN)) {
		if (mario->vy == 0) {
			mario->ChangeState(PlayerCrouchingState::GetInstance());
			return;
		}
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		mario->nx = -1;
	}
}

void PlayerFallingState::OnKeyDown(int KeyCode)
{
	/*Mario* mario = Mario::GetInstance();
	PlayerState* newState;
	if (mario->vy == 0) {
		if (mario->GetIsCrouChing() && mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->ChangeState(PlayerCrouchingState::GetInstance());
		else if (abs(mario->vx) > MARIO_WALKING_SPEED)
		{
			mario->ChangeState(PlayerRunningState::GetInstance());
		}
		else {
			mario->ChangeState(PlayerStandingState::GetInstance());
		}
	}*/
}

PlayerState* PlayerFallingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerFallingState();
	Mario* mario = Mario::GetInstance();
	return __instance;
}

PlayerFallingState::~PlayerFallingState()
{
}
