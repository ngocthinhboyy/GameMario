#include "PlayerHighJumpingState.h"
#include "Mario.h"
#include "PlayerFallingState.h"
#include "game.h"
#include "PlayerWalkingState.h"
#include "debug.h"
#include "PlayerSpinningState.h"
#include "PlayerFallingSlowlyState.h"
#include "PlayerThrowingFireballState.h"

PlayerHighJumpingState::PlayerHighJumpingState()
{
}
PlayerState* PlayerHighJumpingState::__instance = NULL;
void PlayerHighJumpingState::SetAnimation()
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
				animationID = MARIO_ANI_BIG_HIGH_JUMP_RIGHT;
			else
				animationID = MARIO_ANI_BIG_HIGH_JUMP_LEFT;
		}
		break;
	}
	case MARIO_LEVEL_SMALL:
		if (mario->nx == 1)
			animationID = MARIO_ANI_SMALL_HIGH_JUMP_RIGHT;
		else
			animationID = MARIO_ANI_SMALL_HIGH_JUMP_LEFT;
		break;
	case MARIO_LEVEL_RACCOON:
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_RACCOON_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_RACCOON_HIGH_JUMP_RIGHT;
			else
				animationID = MARIO_ANI_RACCOON_HIGH_JUMP_LEFT;
		}
		break;
	case MARIO_LEVEL_FIRE:
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_FIRE_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_FIRE_HIGH_JUMP_RIGHT;
			else
				animationID = MARIO_ANI_FIRE_HIGH_JUMP_LEFT;
		}
		break;
	default:
		break;
	}
}

void PlayerHighJumpingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (abs(mario->vy) < MARIO_JUMP_MAX_SPEED_Y && !stopIncreasingSpeed) {
		mario->vy += (dt * -MARIO_ACCELERATION_JUMP_Y);
	}
	if (mario->vy >= 0) {
		stopIncreasingSpeed = false;
		if(mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
		else
			mario->ChangeState(PlayerFallingState::GetInstance());
	}

}
void PlayerHighJumpingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	//DebugOut(L"mario VX %f\n", mario->vx);
	if (mario->vy >= 0) {
		stopIncreasingSpeed = false;
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
		else
			mario->ChangeState(PlayerFallingState::GetInstance());
	}
	if (game->IsKeyDown(DIK_S)) {
		if (abs(mario->vy) >= MARIO_JUMP_MAX_SPEED_Y)
			stopIncreasingSpeed = true;
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (abs(mario->vx) == 0) {
			mario->vx = MARIO_WALKING_SPEED;
		}
		if (mario->vx < 0) {
			mario->vx = -0.3 * (mario->vx);
		}
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		
		if (abs(mario->vx) == 0) {
			mario->vx = -MARIO_WALKING_SPEED;
		}
		if (mario->vx > 0) {
			mario->vx = -0.3 * (mario->vx);
		}
		mario->nx = -1;
	}
}

void PlayerHighJumpingState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_A: {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			stopIncreasingSpeed = false;
			mario->ChangeState(PlayerSpinningState::GetInstance());
		}
		else if (mario->GetLevel() == MARIO_LEVEL_FIRE){
			stopIncreasingSpeed = false;
			mario->ChangeState(PlayerThrowingFireballState::GetInstance()); 
		}
		break;
	}
	default:
		break;
	}
}

void PlayerHighJumpingState::OnKeyUp(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_S: {
		Mario* mario = Mario::GetInstance();
		stopIncreasingSpeed = true;
		if (mario->vy > 0) {
			stopIncreasingSpeed = false;
			if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
				mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
			else
				mario->ChangeState(PlayerFallingState::GetInstance());
		}
		break;
	}
	default:
		break;
	};
}

PlayerState* PlayerHighJumpingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerHighJumpingState();
	SetAnimation();
	return __instance;
}

PlayerHighJumpingState::~PlayerHighJumpingState()
{
}
