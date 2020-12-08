#include "PlayerStandingState.h"
#include "Mario.h"
#include "Game.h"
#include "debug.h"
#include "PlayerState.h"
#include "PlayerWalkingState.h"
#include "PlayerJumpingState.h"
#include "PlayerHighJumpingState.h"
#include "PlayerCrouchingState.h"
#include "PlayerRunningState.h"
#include "PlayerSpinningState.h"
#include "PlayerThrowingFireballState.h"
#include "PlayerBonusTransformState.h"
#include "PlayerMovingDownAndUpState.h"


PlayerState* PlayerStandingState::__instance = NULL;
PlayerState* PlayerStandingState::GetInstance() {
	
	if (__instance == NULL) __instance = new PlayerStandingState();
	Mario* mario = Mario::GetInstance();
	mario->vx = 0;
	mario->SetIsOnGround(true);
	SetAnimation();
	return __instance;
}
PlayerStandingState::PlayerStandingState()
{
}
void PlayerStandingState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_IDLE_RIGHT;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		if (mario->nx == 1)
			animationID = MARIO_ANI_SMALL_IDLE_RIGHT;
		else
			animationID = MARIO_ANI_SMALL_IDLE_LEFT;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		if (mario->nx == 1)
			animationID = MARIO_ANI_RACCOON_IDLE_RIGHT;
		else
			animationID = MARIO_ANI_RACCOON_IDLE_LEFT;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_IDLE_RIGHT;
		break;
	}
	default:
		break;
	}
}
void PlayerStandingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->vy > 0) {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			mario->ChangeState(PlayerFallingState::GetInstance());
			return;
		}
		mario->ChangeState(PlayerFallingState::GetInstance());
	}
}
void PlayerStandingState::OnKeyDown(int KeyCode) {
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_1:
		mario->ChangeState(PlayerBonusTransformState::GetInstance());
		break;
	case DIK_X:
			mario->vy = -MARIO_JUMP_SPEED_Y;
			mario->ChangeState(PlayerJumpingState::GetInstance());
		break;
	case DIK_S:
			mario->vy = -MARIO_JUMP_SPEED_Y;
			mario->ChangeState(PlayerHighJumpingState::GetInstance());
		break;
	case DIK_DOWN:
	{		if (mario->GetCanGoDownIntoGate()) {
				mario->ChangeState(PlayerMovingDownAndUpState::GetInstance());
			}
			else if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
				mario->y += MARIO_DEVIATION_CROUCHING_Y;
				mario->ChangeState(PlayerCrouchingState::GetInstance());
			}
		break;
	}
	case DIK_A: {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->ChangeState(PlayerSpinningState::GetInstance());
		else if (mario->GetLevel() == MARIO_LEVEL_FIRE) {
			mario->ChangeState(PlayerThrowingFireballState::GetInstance());
		}
		break;
	}
	default:
		break;
	}
}
void PlayerStandingState::KeyState(BYTE* states) {
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_X)) {
		mario->vy = -MARIO_JUMP_SPEED_Y;
		mario->ChangeState(PlayerJumpingState::GetInstance());
	}
	else if ((game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT))) {
		/*if (game->IsKeyDown(DIK_DOWN))
			mario->ChangeState(PlayerCrouchingState::GetInstance());*/
		return;
	}
	else if (game->IsKeyDown(DIK_A) && (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))) {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			mario->ChangeState(PlayerSpinningState::GetInstance());
			return;
		}
		mario->ChangeState(PlayerRunningState::GetInstance());
		return;
	}
	else if (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT)) {
			mario->ChangeState(PlayerWalkingState::GetInstance());
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (mario->GetCanGoDownIntoGate()) {
			mario->ChangeState(PlayerMovingDownAndUpState::GetInstance());
			return;
		}
		else if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
			mario->y += MARIO_DEVIATION_CROUCHING_Y;
			mario->ChangeState(PlayerCrouchingState::GetInstance());
			return;
		}
	}
	else if (game->IsKeyDown(DIK_Z) && mario->GetLevel() == MARIO_LEVEL_RACCOON) {
		mario->ChangeState(PlayerSpinningState::GetInstance());
		return;
	}
}
PlayerStandingState::~PlayerStandingState() {};