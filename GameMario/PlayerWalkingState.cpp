#include "PlayerWalkingState.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "Mario.h"
#include "Game.h"
#include "PlayerJumpingState.h"
#include "PlayerHighJumpingState.h"
#include "PlayerRunningState.h"


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
		default:
			break;
		}
}
void PlayerWalkingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
}
void PlayerWalkingState::OnKeyDown(int KeyCode) {

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
		default:
			mario->ChangeState(PlayerStandingState::GetInstance());
			break;
	}
}
void PlayerWalkingState::KeyState(BYTE* states) {
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_X)) {
		mario->vy = -MARIO_JUMP_SPEED_Y;
		mario->ChangeState(PlayerJumpingState::GetInstance());
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		mario->vx = MARIO_WALKING_SPEED;
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		mario->vx = -MARIO_WALKING_SPEED;
		mario->nx = -1;
	}
	else
		mario->ChangeState(PlayerStandingState::GetInstance());
}
PlayerWalkingState::~PlayerWalkingState() {};