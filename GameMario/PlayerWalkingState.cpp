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


PlayerState* PlayerWalkingState::__instance = NULL;
PlayerState* PlayerWalkingState::GetInstance() {
	if (__instance == NULL) __instance = new PlayerWalkingState();
	return __instance;
}
PlayerWalkingState::PlayerWalkingState() {}
void PlayerWalkingState::SetAnimation(int levelPlayer)
{
	Mario* mario = Mario::GetInstance();
		switch (levelPlayer) {
		case MARIO_LEVEL_BIG:
		{
			if (mario->nx == 1)
				animationID = MARIO_ANI_BIG_WALKING_RIGHT;
			else
				animationID = MARIO_ANI_BIG_WALKING_LEFT;
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
			break;
		}
		case MARIO_LEVEL_FIRE:
		{
			if (mario->nx == 1)
				animationID = MARIO_ANI_FIRE_WALKING_RIGHT;
			else
				animationID = MARIO_ANI_FIRE_WALKING_LEFT;
			break;
		}
		default:
			break;
		}
}
void PlayerWalkingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (isSlow) {
		mario->vx -= MARIO_SLOWLY_SPEED_ACCECLERATION *dt* mario->nx;
	}
	if (mario->vx * mario->nx <= 0 && isSlow) {
		mario->vx = 0;
		isSlow = false;
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
				mario->ChangeState(PlayerJumpingState::GetInstance());
			break;
		case DIK_S:
				mario->vy = -MARIO_JUMP_SPEED_Y;
				isSlow = false;
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
	SetAnimation(mario->GetLevel());
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_X)) {
			mario->vy = -MARIO_JUMP_SPEED_Y;
			isSlow = false;
			mario->ChangeState(PlayerJumpingState::GetInstance());
		return;
	}
	if (game->IsKeyDown(DIK_A)) {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			isSlow = false;
			mario->ChangeState(PlayerSpinningState::GetInstance());
			return;
		}
		else if (mario->GetLevel() == MARIO_LEVEL_FIRE) {
			isSlow = false;
			mario->ChangeState(PlayerThrowingFireballState::GetInstance());
			return;
		}
		isSlow = false;
		mario->ChangeState(PlayerRunningState::GetInstance());
		return;
	}
	if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
		isSlow = true;
		//mario->ChangeState(PlayerStandingState::GetInstance());
		return;
	} else if (game->IsKeyDown(DIK_RIGHT)) {
		mario->vx = MARIO_WALKING_SPEED;
		if (game->IsKeyDown(DIK_Z) && mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			isSlow = false;
			mario->ChangeState(PlayerSpinningState::GetInstance());
		}
		mario->nx = 1;
	} else if (game->IsKeyDown(DIK_LEFT)) {
		mario->vx = -MARIO_WALKING_SPEED;
		if (game->IsKeyDown(DIK_Z) && mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			isSlow = false;
			mario->ChangeState(PlayerSpinningState::GetInstance());
		}
		mario->nx = -1;
	}
	else {
		isSlow = true;
		//mario->ChangeState(PlayerStandingState::GetInstance());
	}
}
PlayerWalkingState::~PlayerWalkingState() {};