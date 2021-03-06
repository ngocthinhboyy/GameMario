#include "PlayerDieState.h"
#include "Mario.h"
#include "game.h"
#include "PlayScene.h"
#include "debug.h"
#include "Camera.h"

PlayerDieState::PlayerDieState()
{
}

void PlayerDieState::SetAnimation()
{
	switch (Mario::GetInstance()->GetLevel()) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_DIE;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_DIE;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_DIE;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_DIE;
		break;
	}
	default:
		break;
	}
}

void PlayerDieState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
		if (GetTickCount64() - startTimeDie <= 600) {
			mario->vy = 0;
		}
		else if (GetTickCount64() - startTimeDie <= 1000) {
			mario->vy = -0.35f;
		}
		else if (GetTickCount64() - startTimeDie >= 4000) {
			mario->vy = 0.8f;
			mario->vx = 0;
			if (mario->GetHeart() == 1) {
				mario->SetHeart(4);
			}
			else
				mario->SetHeart(mario->GetHeart() - 1);
			Mario::GetInstance()->SetUntouchable(false);
			Mario::GetInstance()->SetIsGrowingUp(false);
			Mario::GetInstance()->Reset();
			Game::GetInstance()->SwitchScene(1);
		}
		else {
			mario->vy = 0.8f;
		}
	}
	else {
		if (GetTickCount64() - startTimeDie >= 3000) {
			mario->vy = 0;
			mario->vx = 0;
			if (mario->GetHeart() == 1)
				mario->SetHeart(4);
			else
				mario->SetHeart(mario->GetHeart() - 1);
			mario->SetLevel(MARIO_LEVEL_SMALL);
			Mario::GetInstance()->SetUntouchable(false);
			Mario::GetInstance()->SetIsGrowingUp(false);
			Mario::GetInstance()->Reset();
			Game::GetInstance()->SwitchScene(1);
		}
	}
	Camera::GetInstance()->SetCamXSpeed(0);
	mario->dy = mario->vy * dt;
	mario->y += mario->dy;
	mario->noCollisionConsideration = true;
}

void PlayerDieState::KeyState(BYTE* states)
{

}

PlayerState* PlayerDieState::__instance = NULL;
DWORD PlayerDieState::startTimeDie = 0;
PlayerState* PlayerDieState::GetInstance()
{
    if (__instance == NULL) __instance = new PlayerDieState();
	startTimeDie = GetTickCount64();
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	SetAnimation();
	scene->StopGame(5000);
    return __instance;
}

PlayerDieState::~PlayerDieState()
{
}
