#include "PlayerJumpingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "PlayerFallingState.h"
#include "PlayerWalkingState.h"
#include "PlayerCrouchingState.h"
#include "PlayerSpinningState.h"

PlayerJumpingState::PlayerJumpingState() {};
PlayerJumpingState::~PlayerJumpingState() {};

PlayerState* PlayerJumpingState::__instance = NULL;
PlayerState* PlayerJumpingState::GetInstance() {
	if (__instance == NULL) __instance = new PlayerJumpingState();
	SetAnimation();
	Mario::GetInstance()->SetIsOnGround(false);
	return __instance;
};

void PlayerJumpingState::SetAnimation() {
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel())
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
	case MARIO_LEVEL_FIRE:
		if (mario->GetIsCrouChing()) {
			animationID = MARIO_ANI_FIRE_CROUCH;
		}
		else {
			if (mario->nx == 1)
				animationID = MARIO_ANI_FIRE_JUMP_RIGHT;
			else
				animationID = MARIO_ANI_FIRE_JUMP_LEFT;
		}
		break;
	default:
		break;
	}
}

void PlayerJumpingState::Update(int dt)
{
	//DebugOut(L"JUMPONGGGGGGGGGGG \n");
	Mario* mario = Mario::GetInstance();
	//DebugOut(L"JUMPONGGGGGGGGGGG %d\n", animationID);
	if (mario->vy > 0)
		mario->ChangeState(PlayerFallingState::GetInstance());
}
void PlayerJumpingState::KeyState(BYTE* states) {
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_X))
	{
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (abs(mario->vx) == 0) {
				mario->vx = MARIO_WALKING_SPEED;
			}
			if (mario->vx < 0) {
				mario->vx = -mario->vx;
			}
			mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (abs(mario->vx) == 0) {
				mario->vx = -MARIO_WALKING_SPEED;
			}
			if (mario->vx > 0) {
				mario->vx = -mario->vx;
			}
			mario->nx = -1;
		}
	}
}

void PlayerJumpingState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_A: {
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->ChangeState(PlayerSpinningState::GetInstance());
		break;
	}
	default:
		break;
	}
}
