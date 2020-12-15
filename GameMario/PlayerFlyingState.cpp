#include "PlayerFlyingState.h"
#include "PlayerRunningState.h"
#include "PlayerFallingSlowlyState.h"
#include "Mario.h"
#include "game.h"
#include "debug.h"

PlayerState* PlayerFlyingState::__instance = NULL;
DWORD PlayerFlyingState::timeStartFlying = 0;
PlayerFlyingState::PlayerFlyingState()
{
}

void PlayerFlyingState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()){
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_FLY;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_FLY;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_FLY;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_FLY;
		break;
	}
	default:
		break;
	}
}

void PlayerFlyingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
		if (GetTickCount64() - timeStartFlying >= 4000) {
			stopIncreasingSpeed = false;
			mario->SetIsFlying(false);
			mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
			mario->vx = MARIO_WALKING_SPEED * mario->nx;
			return;
		}
	}
	if (abs(mario->vy) < MARIO_JUMP_MAX_SPEED_Y && !stopIncreasingSpeed) {
		mario->vy += (dt * -MARIO_ACCELERATION_JUMP_Y);
	}
	if (mario->vy == 0) {
		stopIncreasingSpeed = false;
		mario->SetIsFlying(false);
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			//mario->vx = MARIO_RUNNING_MAX_SPEED * mario->nx;
		}
		mario->ChangeState(PlayerRunningState::GetInstance());
	}
}

void PlayerFlyingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_S)) {
		if (abs(mario->vy) >= MARIO_JUMP_MAX_SPEED_Y)
			stopIncreasingSpeed = true;
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			mario->vx =0.35f;
		}
		else {
			if (mario->vx < 0) {
				mario->vx = -0.3 * (mario->vx);
			}
		}
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			mario->vx = -0.35f;
		}
		else {
			if (mario->vx > 0) {
				mario->vx = -0.3 * (mario->vx);
			}
		}
		mario->nx = -1;
	}
}

void PlayerFlyingState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			stopIncreasingSpeed = true;
			DWORD now = GetTickCount64();
			if (now - timeStartFlying < 4000) {
				mario->vy = -0.4f;
			}
		}
		break;
	default:
		break;
	}
}

void PlayerFlyingState::OnKeyUp(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_S: {
		Mario* mario = Mario::GetInstance();
		stopIncreasingSpeed = true;
		if (mario->vy == 0) {
			stopIncreasingSpeed = false;
			mario->SetIsFlying(false);
			mario->ChangeState(PlayerRunningState::GetInstance());
		}
		break;
	}
	default:
		break;
	};
}

PlayerState* PlayerFlyingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerFlyingState();
	timeStartFlying = GetTickCount64();
	SetAnimation();
	Mario::GetInstance()->SetIsFlying(true);
	return __instance;
}

PlayerFlyingState::~PlayerFlyingState()
{
}
