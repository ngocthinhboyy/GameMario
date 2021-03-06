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
#include "Camera.h"
#include "PlayerFallingSlowlyState.h"
#include "PlayerLevelDownTransformState.h"


PlayerState* PlayerStandingState::__instance = NULL;
PlayerState* PlayerStandingState::GetInstance() {
	
	if (__instance == NULL) __instance = new PlayerStandingState();
	Mario* mario = Mario::GetInstance();
	mario->vx = 0;
	//mario->SetIsOnGround(true);
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
}
void PlayerStandingState::OnKeyDown(int KeyCode) {
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_1: {
		mario->SetLevel(MARIO_LEVEL_SMALL);
		mario->y -= 40;
		SetAnimation();
		break;
	}
	case DIK_2: {
		mario->SetLevel(MARIO_LEVEL_SMALL);
		SetAnimation();
		mario->ChangeState(PlayerBonusTransformState::GetInstance());
		break;
	}
	case DIK_3: {
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			mario->y -= 40;
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->ChangeState(PlayerBonusTransformState::GetInstance());
		break;
	}
	case DIK_4: {
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			mario->y -= 40;
		mario->SetLevel(MARIO_LEVEL_FIRE);
		SetAnimation();
		break;
	}
	case DIK_5: {
		mario->x = 6792;
		mario->y = 250;
		Camera::GetInstance()->SetLockCamY(true);
		SetAnimation();
		break;
	}
	case DIK_6: {
		mario->x = 4250;
		mario->y = 250;
		Camera::GetInstance()->SetLockCamY(true);
		SetAnimation();
		break;
	}
	case DIK_7: {
		mario->x = 5690;
		mario->y = 1100;
		float camx  = mario->x - (Game::GetInstance()->GetScreenWidth() / 2);
		Camera::GetInstance()->SetCamPos(camx, CAM_Y_DRAW_MAP);
		Camera::GetInstance()->SetLockCamY(false);
		SetAnimation();
		break;
	}
	case DIK_8: {
		mario->x = 5831.4;
		mario->y = 924;
		mario->nx = -1;
		Game* game = Game::GetInstance();
		PlayScene* playScene = dynamic_cast<PlayScene*> (game->GetCurrentScene());
		Camera::GetInstance()->SetLockCamY(true);
		Camera::GetInstance()->SetCamPos( 6160 - game->GetScreenWidth(), 720.0f);
		SetAnimation();
		break;
	}
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
		return;
	}
	else if (game->IsKeyDown(DIK_A) && (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))) {

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