#include "PlayerGoingByCameraState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerWalkingState.h"
#include "debug.h"

PlayerGoingByCameraState::PlayerGoingByCameraState()
{
}

void PlayerGoingByCameraState::SetAnimation()
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

void PlayerGoingByCameraState::Update(int dt)
{
	Mario::GetInstance()->vx = 0.09f;
}

void PlayerGoingByCameraState::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_RIGHT)) {
		Mario::GetInstance()->SetIsGoingByCamera(false);
		Mario::GetInstance()->nx = 1;
		Mario::GetInstance()->vx = MARIO_WALKING_SPEED;
		Mario::GetInstance()->ChangeState(PlayerWalkingState::GetInstance());
	}
}

void PlayerGoingByCameraState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_DOWN: {
		if (!mario->GetIsCrouChing()) {
			mario->y += MARIO_DEVIATION_CROUCHING_Y;
			mario->SetIsCrouching(true);
			SetAnimation();
		}
		break;
	}
	default:
		break;
	}
}
void PlayerGoingByCameraState::OnKeyUp(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_DOWN: {
		if (mario->GetIsCrouChing()) {
			mario->y -= MARIO_DEVIATION_CROUCHING_Y;
			mario->SetIsCrouching(false);
			SetAnimation();
		}
		break;
	}
	default:
		break;
	}
}
PlayerState* PlayerGoingByCameraState::__instance = NULL;
PlayerState* PlayerGoingByCameraState::GetInstance()
{
	Mario::GetInstance()->SetIsGoingByCamera(true);
	if (__instance == NULL) __instance = new PlayerGoingByCameraState();
	SetAnimation();
	return __instance;
}

PlayerGoingByCameraState::~PlayerGoingByCameraState()
{
}
