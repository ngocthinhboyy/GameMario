#include "PlayerFallingSlowlyState.h"
#include "PlayerStandingState.h"
#include "Mario.h"
#include "Game.h"

PlayerState* PlayerFallingSlowlyState::__instance = NULL;
PlayerFallingSlowlyState::PlayerFallingSlowlyState()
{
}

void PlayerFallingSlowlyState::SetAnimation(int levelPlayer)
{
	Mario* mario = Mario::GetInstance();
	switch (levelPlayer)
	{
	case MARIO_LEVEL_RACCOON:
		animationID = MARIO_ANI_RACCOON_FALLING_SLOWLY;
		break;
	default:
		break;
	}
}

void PlayerFallingSlowlyState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if(false)
		mario->vy += -mario->vy*0.7;
	if (mario->vy == 0) {
		timePress = 0;
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

void PlayerFallingSlowlyState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
	Game* game = Game::GetInstance();
	if (game->IsKeyDown(DIK_S)) {
		DWORD now = GetTickCount();
		if(mario->vy > 0 && now-timePress < 250)
			mario->vy += -mario->vy * 0.4f;
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (mario->vx < 0)
			mario->vx = -mario->vx;
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (mario->vx > 0)
			mario->vx = -mario->vx;
		mario->nx = -1;
	}
}

void PlayerFallingSlowlyState::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_S: {
		timePress = GetTickCount();
		break;
	}
	default:
		break;
	}
}

void PlayerFallingSlowlyState::OnKeyUp(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_S: {
		timePress = GetTickCount();
		break;
	}
	default:
		break;
	}
}

PlayerState* PlayerFallingSlowlyState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerFallingSlowlyState();
	return __instance;
}

PlayerFallingSlowlyState::~PlayerFallingSlowlyState()
{
}
