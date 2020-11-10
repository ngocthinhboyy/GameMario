#include "PlayerFlyingState.h"
#include "PlayerRunningState.h"
#include "Mario.h"
#include "game.h"
#include "debug.h"

PlayerState* PlayerFlyingState::__instance = NULL;
DWORD PlayerFlyingState::timeStartFlying = 0;
PlayerFlyingState::PlayerFlyingState()
{
}

void PlayerFlyingState::SetAnimation(int levelPlayer)
{
	Mario* mario = Mario::GetInstance();
	switch (levelPlayer) {
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
	if (abs(mario->vy) < MARIO_JUMP_MAX_SPEED_Y && !stopIncreasingSpeed) {
		mario->vy += (dt * -MARIO_ACCELERATION_JUMP_Y);
	}
	if (mario->vy == 0) {
		stopIncreasingSpeed = false;
		mario->ChangeState(PlayerRunningState::GetInstance());
	}
}

void PlayerFlyingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_S)) {
		if (abs(mario->vy) >= MARIO_JUMP_MAX_SPEED_Y)
			stopIncreasingSpeed = true;
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (mario->vx < 0) {
			mario->vx = -0.3*(mario->vx);
		}
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (mario->vx > 0) {
			mario->vx = -0.3 * (mario->vx);
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
			DWORD now = GetTickCount();
			if (now - timeStartFlying < 1000) {
				mario->vy += -0.7f;
			}
			else {

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
	timeStartFlying = GetTickCount();
	return __instance;
}

PlayerFlyingState::~PlayerFlyingState()
{
}
