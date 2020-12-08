#include "PlayerWalkingState.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "Mario.h"
#include "Game.h"
#include "PlayerJumpingState.h"
#include "PlayerHighJumpingState.h"
#include "PlayerRunningState.h"
#include "PlayerSpinningState.h"
#include "PlayerThrowingFireballState.h"
#include "PlayerCrouchingState.h"
#include "PlayerSkiddingState.h"
#include "PlayerFallingSlowlyState.h"


PlayerState* PlayerWalkingState::__instance = NULL;
bool PlayerWalkingState::prevKeyIsLeft = false;
bool PlayerWalkingState::prevKeyIsRight = false;
PlayerState* PlayerWalkingState::GetInstance() {
	if (__instance == NULL) __instance = new PlayerWalkingState();
	prevKeyIsLeft = false;
	prevKeyIsRight = false;
	SetAnimation();
	return __instance;
}
PlayerWalkingState::PlayerWalkingState() {}
void PlayerWalkingState::SetAnimation()
{
		Mario* mario = Mario::GetInstance();
		switch (mario->GetLevel()) {
		case MARIO_LEVEL_BIG:
		{
			if (mario->nx == 1)
				animationID = MARIO_ANI_BIG_WALKING_RIGHT;
			else
				animationID = MARIO_ANI_BIG_WALKING_LEFT;
			if (mario->GetIsCrouChing()) {
				animationID = MARIO_ANI_BIG_CROUCH;
			}
			break;
		}
		case MARIO_LEVEL_SMALL:
		{
			if (mario->nx == 1)
				animationID = MARIO_ANI_SMALL_WALKING_RIGHT;
			else
				animationID = MARIO_ANI_SMALL_WALKING_LEFT;
			break;
		}
		case MARIO_LEVEL_RACCOON:
		{
			if (mario->nx == 1)
				animationID = MARIO_ANI_RACCOON_WALKING_RIGHT;
			else
				animationID = MARIO_ANI_RACCOON_WALKING_LEFT;
			if (mario->GetIsCrouChing()) {
				animationID = MARIO_ANI_RACCOON_CROUCH;
			}
			break;
		}
		case MARIO_LEVEL_FIRE:
		{
			if (mario->nx == 1)
				animationID = MARIO_ANI_FIRE_WALKING_RIGHT;
			else
				animationID = MARIO_ANI_FIRE_WALKING_LEFT;
			if (mario->GetIsCrouChing()) {
				animationID = MARIO_ANI_FIRE_CROUCH;
			}
			break;
		}
		default:
			break;
		}
}
void PlayerWalkingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->vy > 0) {
		isSlow = false;
		mario->SetIsCrouching(false);
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
			return;
		}
		mario->ChangeState(PlayerFallingState::GetInstance());
		return;
	}
	if (isSlow) {
		mario->vx -= MARIO_SLOWLY_SPEED_ACCECLERATION *dt* mario->nx;
	}
	if (mario->vx * mario->nx <= 0 && isSlow) {
		mario->vx = 0;
		isSlow = false;
		if (mario->GetIsCrouChing()) {
			mario->ChangeState(PlayerCrouchingState::GetInstance());
			return;
		}
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}
void PlayerWalkingState::OnKeyDown(int KeyCode) {

	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
		case DIK_X:
				mario->vy = -MARIO_JUMP_SPEED_Y;
				isSlow = false;
				mario->SetIsCrouching(false);
				mario->ChangeState(PlayerJumpingState::GetInstance());
			break;
		case DIK_S:
				mario->vy = -MARIO_JUMP_SPEED_Y;
				isSlow = false;
				mario->SetIsCrouching(false);
				mario->ChangeState(PlayerHighJumpingState::GetInstance());
			break;
		/*case DIK_A:
			if(mario->GetLevel() == MARIO_LEVEL_RACCOON)
				mario->ChangeState(PlayerSpinningState::GetInstance());
			break;*/
		default:
			break;
	}
}
void PlayerWalkingState::KeyState(BYTE* states) {
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_X)) {
		mario->vy = -MARIO_JUMP_SPEED_Y;
		isSlow = false;
		mario->SetIsCrouching(false);
		mario->ChangeState(PlayerJumpingState::GetInstance());
		return;
	}
	if (game->IsKeyDown(DIK_A)) {
		if (!mario->GetIsCrouChing()) {
			if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
				isSlow = false;
				mario->SetIsCrouching(false);
				mario->ChangeState(PlayerSpinningState::GetInstance());
				return;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_FIRE) {
				isSlow = false;
				mario->SetIsCrouching(false);
				mario->ChangeState(PlayerThrowingFireballState::GetInstance());
				return;
			}
			isSlow = false;
			mario->SetIsCrouching(false);
			mario->ChangeState(PlayerRunningState::GetInstance());
			return;
		}
	}
	if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
		isSlow = true;
		//DebugOut(L"EEEEEE %f\n", mario->vx);
		//if (game->IsKeyDown(DIK_DOWN) && mario->vx == 0) {
		//	if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
		//		mario->SetIsCrouching(true);
		//		SetAnimation();
		//	}
		//	//mario->ChangeState(PlayerStandingState::GetInstance());
		//	return;
		//}
	}
	else if (game->IsKeyDown(DIK_RIGHT)) {
		prevKeyIsRight = true;
		if (mario->GetIsCrouChing()) {
			mario->SetIsCrouching(false);
			mario->y -= MARIO_DEVIATION_CROUCHING_Y;
			SetAnimation();
		}
		if (prevKeyIsLeft) {
			mario->ChangeState(PlayerSkiddingState::GetInstance());
			return;
		}
		else {
			mario->vx = MARIO_WALKING_SPEED;
		}
		if (game->IsKeyDown(DIK_Z) && mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			isSlow = false;
			mario->SetIsCrouching(false);
			mario->ChangeState(PlayerSpinningState::GetInstance());
		}
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		prevKeyIsLeft = true;
		if (mario->GetIsCrouChing()) {
			mario->SetIsCrouching(false);
			mario->y -= MARIO_DEVIATION_CROUCHING_Y;
			SetAnimation();
		}
		if (prevKeyIsRight) {
			mario->ChangeState(PlayerSkiddingState::GetInstance());
			return;
		}
		else {
			mario->vx = -MARIO_WALKING_SPEED;
		}
		if (game->IsKeyDown(DIK_Z) && mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			isSlow = false;
			mario->SetIsCrouching(false);
			mario->ChangeState(PlayerSpinningState::GetInstance());
		}
		mario->nx = -1;
	}
	else if (game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT)) {
		isSlow = true;
		if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
			if (!mario->GetIsCrouChing()) {
				mario->y += MARIO_DEVIATION_CROUCHING_Y;
				mario->SetIsCrouching(true);
				SetAnimation();
			}
		}
		return;
	}
	else {
		isSlow = true;
		//mario->ChangeState(PlayerStandingState::GetInstance());
	}
}
PlayerWalkingState::~PlayerWalkingState() {
};