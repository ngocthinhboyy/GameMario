#include "PlayerBonusTransformState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "debug.h"

PlayerState* PlayerBonusTransformState::__instance = NULL;
PlayerBonusTransformState::PlayerBonusTransformState()
{
}

void PlayerBonusTransformState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()) {
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_EFFECT_GROW_UP;
		break;
	}
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_EFFECT_HAVE_BONUS;
		break;
	}
	default:
		break;
	}
}

void PlayerBonusTransformState::Update(int dt)
{
}

void PlayerBonusTransformState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	bool isLastFrame = animation->GetIsLastFrame();
	/*if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
		DWORD now = GetTickCount64();
		if (now - startTransform >= 500) {
			mario->y -= 12;
		}
	}*/
	if (isLastFrame) {
		animation->ResetAnimation();
		mario->SetLevel(mario->GetLevel() + 1);
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

DWORD PlayerBonusTransformState::startTransform = 0;
PlayerState* PlayerBonusTransformState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerBonusTransformState();
	SetAnimation();
	startTransform = GetTickCount64();
	return __instance;
}

PlayerBonusTransformState::~PlayerBonusTransformState()
{
}
