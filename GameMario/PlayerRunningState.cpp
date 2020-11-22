#include "PlayerRunningState.h"
#include "game.h"
#include "Mario.h"
#include "PlayerWalkingState.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "PlayerSkiddingState.h"
#include "PlayerJumpingState.h"
#include "PlayerHighJumpingState.h"
#include "PlayerSpinningState.h"
#include "PlayerFlyingState.h"
#include "PlayerCrouchingState.h"

PlayerState* PlayerRunningState::__instance = NULL;
bool PlayerRunningState::lastStateIsSkidding = false;
PlayerRunningState::PlayerRunningState()
{
}

void PlayerRunningState::SetAnimation(int levelPlayer)
{
	switch (levelPlayer)
	{
	case MARIO_LEVEL_BIG: {
		if(isMaxSpeed)
			animationID = MARIO_ANI_BIG_RUN;
		else
			animationID = MARIO_ANI_BIG_WALKING_RIGHT;
		break;
	}
	case MARIO_LEVEL_SMALL: {
		if (isMaxSpeed)
			animationID = MARIO_ANI_SMALL_RUN;
		else
			animationID = MARIO_ANI_SMALL_WALKING_RIGHT;
		break;
	}
	case MARIO_LEVEL_RACCOON: {
		if (isMaxSpeed)
			animationID = MARIO_ANI_RACCOON_RUN;
		else
			animationID = MARIO_ANI_RACCOON_WALKING_RIGHT;
		break;
	}
	case MARIO_LEVEL_FIRE: {
		if (isMaxSpeed)
			animationID = MARIO_ANI_FIRE_RUN;
		else
			animationID = MARIO_ANI_FIRE_WALKING_RIGHT;
		break;
	}
	default:
		break;
	}
}

void PlayerRunningState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if ((abs(mario->vx) < MARIO_RUNNING_MAX_SPEED) && !isMaxSpeed && increaseSpeed && !isSkidding) {
		mario->vx += (dt * MARIO_SPEED_ACCELERATION * (mario->nx));
	}
	if (!increaseSpeed && mario->vx != 0 && !isSkidding) {
		mario->vx += (dt * MARIO_SPEED_ACCELERATION * - (mario->nx));
	}
	if (isSkidding && lastStateIsSkidding) {
		return;
	}
	if ((mario->vx)*(mario->nx) <= 0) {
		isMaxSpeed = false;
		increaseSpeed = true;
		prevKeyIsLeft = false;
		prevKeyIsRight = false;
		isSkidding = false;
		mario->SetIsRunning(false);
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

void PlayerRunningState::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
	if (isSkidding) {
		if (lastStateIsSkidding) {
			mario->nx = -mario->nx;
			mario->vx = -mario->vx;
			lastStateIsSkidding = false;
			prevKeyIsLeft = false;
			prevKeyIsRight = false;
			isSkidding = false;
			return;
		}
		mario->ChangeState(PlayerSkiddingState::GetInstance());
		return;
	}
	if (game->IsKeyDown(DIK_A)) {
		if (abs(mario->vx) >= MARIO_RUNNING_MAX_SPEED) {
			isMaxSpeed = true;
		}
		else {
			isMaxSpeed = false;
		}
		if (game->IsKeyDown(DIK_X)) {
			mario->vy = -MARIO_JUMP_SPEED_Y;
			isMaxSpeed = false;
			increaseSpeed = true;
			prevKeyIsLeft = false;
			prevKeyIsRight = false;
			isSkidding = false;
			mario->SetIsRunning(false);
			mario->ChangeState(PlayerJumpingState::GetInstance());
			return;
		}
		if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
			if (mario->vx != 0) {
				increaseSpeed = false;
			}
		}
		else if (game->IsKeyDown(DIK_RIGHT)) {
			if (!prevKeyIsLeft && !isSkidding) {
				increaseSpeed = true;
				mario->nx = 1;
				prevKeyIsRight = true;
				prevKeyIsLeft = false;
			}
			else {
				mario->speedLast = abs(mario->vx);
				isSkidding = true;
				prevKeyIsLeft = false;
				prevKeyIsRight = true;
			}
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (!prevKeyIsRight && !isSkidding) {
				increaseSpeed = true;
				mario->nx = -1;
				prevKeyIsLeft = true;
				prevKeyIsRight = false;
			}
			else {

				mario->speedLast = abs(mario->vx);
				isSkidding = true;
				prevKeyIsRight = false;
				prevKeyIsLeft = true;
			}
		}
		/*else if (game->IsKeyDown(DIK_Z)) {
			isMaxSpeed = false;
			increaseSpeed = true;
			prevKeyIsLeft = false;
			prevKeyIsRight = false;
			isSkidding = false;
			mario->SetIsRunning(false);
			mario->ChangeState(PlayerCrouchingState::GetInstance());
		}*/
		else {
			if (mario->vx != 0) {
				increaseSpeed = false;
			}
		}
	}
	else {
		increaseSpeed = false;
		if (abs(mario->vx) <= MARIO_WALKING_SPEED) {
			isMaxSpeed = false;
			increaseSpeed = true;
			prevKeyIsLeft = false;
			prevKeyIsRight = false;
			isSkidding = false;
			mario->SetIsRunning(false);
			mario->ChangeState(PlayerWalkingState::GetInstance());
		}
	}
}

void PlayerRunningState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_X: {
		mario->vy = -MARIO_JUMP_SPEED_Y;
		isMaxSpeed = false;
		increaseSpeed = true;
		prevKeyIsLeft = false;
		prevKeyIsRight = false;
		isSkidding = false;
		mario->SetIsRunning(false);
		mario->ChangeState(PlayerJumpingState::GetInstance());
		break;
	}
	case DIK_S: {
		mario->vy = -MARIO_JUMP_SPEED_Y;
		isMaxSpeed = false;
		increaseSpeed = true;
		prevKeyIsLeft = false;
		prevKeyIsRight = false;
		isSkidding = false;
		if (abs(mario->vx) >= MARIO_RUNNING_MAX_SPEED) {;
			mario->ChangeState(PlayerFlyingState::GetInstance());
		}
		else {
			mario->ChangeState(PlayerHighJumpingState::GetInstance());
		}
		break;
	}
	case DIK_A: {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			isMaxSpeed = false;
			increaseSpeed = true;
			prevKeyIsLeft = false;
			prevKeyIsRight = false;
			isSkidding = false;
			mario->SetIsRunning(false);
			mario->ChangeState(PlayerSpinningState::GetInstance());
		}
		break;
	}
	default:
		break;
	}
}

PlayerState* PlayerRunningState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerRunningState();
	Mario* mario = Mario::GetInstance();
	mario->SetIsRunning(true);
	return __instance;
}

PlayerRunningState::~PlayerRunningState()
{
}
