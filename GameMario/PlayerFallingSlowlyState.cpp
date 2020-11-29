#include "PlayerFallingSlowlyState.h"
#include "PlayerStandingState.h"
#include "Mario.h"
#include "Game.h"
#include "PlayerCrouchingState.h"
#include "debug.h"

PlayerState* PlayerFallingSlowlyState::__instance = NULL;
PlayerFallingSlowlyState::PlayerFallingSlowlyState()
{
}

void PlayerFallingSlowlyState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel())
	{
	case MARIO_LEVEL_RACCOON:
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_RACCOON_CROUCH;
		}
		else {
			animationID = MARIO_ANI_RACCOON_FALLING_SLOWLY;
		}
		break;
	default:
		break;
	}
}

void PlayerFallingSlowlyState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->vy == 0 && mario->GetIsOnGround()) {
		timePress = 0;
		if (mario->GetIsCrouChing()) {
			mario->ChangeState(PlayerCrouchingState::GetInstance());
		}
		else
			mario->ChangeState(PlayerStandingState::GetInstance());
		return;
	}
}

void PlayerFallingSlowlyState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_S)) {
		DWORD now = GetTickCount64();
		if(mario->vy > 0 && now-timePress < 250)
			mario->vy += -mario->vy * 0.4f;
	}
	/*if (game->IsKeyDown(DIK_DOWN)) {
		if (mario->vy == 0) {
			mario->vx = 0;
			mario->ChangeState(PlayerCrouchingState::GetInstance());
			return;
		}
	}*/
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (mario->vx < 0)
			mario->vx = -mario->vx;
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (mario->vx > 0)
			mario->vx = -mario->vx;
		mario->nx = -1;
	}
}

void PlayerFallingSlowlyState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_S: {
		timePress = GetTickCount64();
		break;
	}
	default:
		break;
	}
}

void PlayerFallingSlowlyState::OnKeyUp(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_S: {
		timePress = GetTickCount64();
		break;
	}
	default:
		break;
	}
}

PlayerState* PlayerFallingSlowlyState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerFallingSlowlyState();
	Mario::GetInstance()->SetCanGoDownIntoGate(false);
	SetAnimation();
	return __instance;
}

PlayerFallingSlowlyState::~PlayerFallingSlowlyState()
{
}
