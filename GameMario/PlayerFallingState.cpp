#include "PlayerFallingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "PlayerJumpingState.h"
#include "debug.h"
#include "PlayerCrouchingState.h"
#include "PlayerRunningState.h"
#include "PlayerSpinningState.h"


PlayerFallingState::PlayerFallingState()
{
}

PlayerState* PlayerFallingState::__instance = NULL;

void PlayerFallingState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel())
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
		// thieu animation fall
	case MARIO_LEVEL_FIRE:
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_FIRE_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_FIRE_IDLE_RIGHT;
			else
				animationID = MARIO_ANI_FIRE_IDLE_LEFT;
		}
		break;
	default:
		break;
	}
}

void PlayerFallingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->vy == 0 && mario->GetIsOnGround()) {
		if (mario->GetIsCrouChing() && mario->GetLevel() != MARIO_LEVEL_SMALL) {
			mario->ChangeState(PlayerCrouchingState::GetInstance());
		}
		else if (abs(mario->vx) > MARIO_WALKING_SPEED)
		{
			mario->ChangeState(PlayerRunningState::GetInstance());
		}
		else {
			mario->ChangeState(PlayerStandingState::GetInstance());
		}
	}
	return;
}
void PlayerFallingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_DOWN)) {
		if(mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->SetIsCrouching(true);
	}
	else {
		mario->SetIsCrouching(false);
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (mario->vx <= 0)
			mario->vx = -mario->vx;
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (mario->vx >= 0)
			mario->vx = -mario->vx;
		mario->nx = -1;
	}
}

void PlayerFallingState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_A: {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			mario->ChangeState(PlayerSpinningState::GetInstance());
		}
		break;
	}
	default:
		break;
	}
}

PlayerState* PlayerFallingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerFallingState();
	SetAnimation();
	return __instance;
}

PlayerFallingState::~PlayerFallingState()
{
}
