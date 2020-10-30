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


PlayerState* PlayerStandingState::__instance = NULL;
PlayerState* PlayerStandingState::GetInstance() {
	
	if (__instance == NULL) __instance = new PlayerStandingState();
	Mario* mario = Mario::GetInstance();
	mario->vx = 0;
	return __instance;
}
void PlayerStandingState::SetAnimation(int levelPlayer)
{
	Mario* mario = Mario::GetInstance();
	switch (levelPlayer) {
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
	default:
		break;
	}
}
void PlayerStandingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
}
void PlayerStandingState::OnKeyDown(int KeyCode) {
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_X:
		mario->vy = -MARIO_JUMP_SPEED_Y;
		mario->ChangeState(PlayerJumpingState::GetInstance());
		break;
	case DIK_S:
		mario->vy = -MARIO_JUMP_SPEED_Y;
		mario->ChangeState(PlayerHighJumpingState::GetInstance());
		break;
	case DIK_DOWN:
	{
		if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
			mario->ChangeState(PlayerCrouchingState::GetInstance());
		}
		break;
	}
	case DIK_A: {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->ChangeState(PlayerSpinningState::GetInstance());
		break;
	}
	default:
		break;
	}
}
void PlayerStandingState::KeyState(BYTE* states) {
	//DebugOut(L"STANDING \n");
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_X)) {
		mario->vy = -MARIO_JUMP_SPEED_Y;
		mario->ChangeState(PlayerJumpingState::GetInstance());
	}
	else if ((game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT))) {
		//DebugOut(L"HAI NUT \n");
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
		if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
			mario->ChangeState(PlayerCrouchingState::GetInstance());
		}
	}
}
PlayerStandingState::PlayerStandingState() {
};
PlayerStandingState::~PlayerStandingState() {};