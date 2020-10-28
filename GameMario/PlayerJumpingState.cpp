#include "PlayerJumpingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "PlayerFallingState.h"
#include "PlayerWalkingState.h"
#include "PlayerCrouchingState.h"

PlayerJumpingState::PlayerJumpingState() {};
PlayerJumpingState::~PlayerJumpingState() {};

PlayerState* PlayerJumpingState::__instance = NULL;
PlayerState* PlayerJumpingState::GetInstance() {
	if (__instance == NULL) __instance = new PlayerJumpingState();
	return __instance;
};

void PlayerJumpingState::SetAnimation(int levelPlayer) {
	Mario* mario = Mario::GetInstance();
	switch (levelPlayer)
	{
	case MARIO_LEVEL_BIG: {
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_BIG_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_BIG_JUMP_RIGHT;
			else
				animationID = MARIO_ANI_BIG_JUMP_LEFT;
		}
		break;
	}
	case MARIO_LEVEL_SMALL:
		if (mario->nx == 1)
			animationID = MARIO_ANI_SMALL_JUMP_RIGHT;
		else
			animationID = MARIO_ANI_SMALL_JUMP_LEFT;
		break;
	case MARIO_LEVEL_RACCOON:
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_RACCOON_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_RACCOON_JUMP_RIGHT;
			else
				animationID = MARIO_ANI_RACCOON_JUMP_LEFT;
		}
		break;
	default:
		break;
	}
}

void PlayerJumpingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
}
void PlayerJumpingState::KeyState(BYTE* states) {
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_X))
	{
		if (game->IsKeyDown(DIK_RIGHT))
			{
				mario->vx = 2*MARIO_WALKING_SPEED;
				mario->nx = 1;
			}
			else if (game->IsKeyDown(DIK_LEFT)) {
				mario->vx = -MARIO_WALKING_SPEED*2;
					mario->nx = -1;
			}
	}
	if(mario->vy > 0)
		mario->ChangeState(PlayerFallingState::GetInstance());
}
