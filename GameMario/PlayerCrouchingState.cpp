#include "PlayerCrouchingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "debug.h"
#include "PlayerHighJumpingState.h"
#include "PlayerJumpingState.h"

PlayerCrouchingState::PlayerCrouchingState()
{
}
PlayerState* PlayerCrouchingState::__instance = NULL;
void PlayerCrouchingState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel())
	{
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_CROUCH;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_CROUCH;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_CROUCH;
		break;
	}
	default:
		break;
	}
}

void PlayerCrouchingState::Update(int dt)
{

}

void PlayerCrouchingState::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	Mario* mario = Mario::GetInstance();
	if (game->IsKeyDown(DIK_DOWN)) {
		if (game->IsKeyDown(DIK_RIGHT)) {
			mario->SetIsCrouching(false);
			mario->y -= MARIO_DEVIATION_CROUCHING_Y;
			mario->ChangeState(PlayerWalkingState::GetInstance());
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			mario->SetIsCrouching(false);
			mario->y = mario->y - MARIO_DEVIATION_CROUCHING_Y;
			mario->ChangeState(PlayerWalkingState::GetInstance());
		}
		else if (game->IsKeyDown(DIK_X)) {
			mario->vy = -MARIO_JUMP_SPEED_Y;
			mario->ChangeState(PlayerJumpingState::GetInstance());
		}
	}
	else {
		mario->SetIsCrouching(false);
		mario->y = mario->y - MARIO_DEVIATION_CROUCHING_Y;
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

void PlayerCrouchingState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_S:
		mario->vy = -MARIO_JUMP_SPEED_Y;
		mario->ChangeState(PlayerHighJumpingState::GetInstance());
		break;
	default:
		break;
	}
}

PlayerState* PlayerCrouchingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerCrouchingState();
	Mario* mario = Mario::GetInstance();
	mario->SetIsCrouching(true);
	mario->vx = 0;
	SetAnimation();
	return __instance;
}

PlayerCrouchingState::~PlayerCrouchingState()
{
}
