#include "PlayerLevelDownTransformState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"


PlayerState* PlayerLevelDownTransformState::__instance = NULL;
PlayerLevelDownTransformState::PlayerLevelDownTransformState()
{
}

void PlayerLevelDownTransformState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_EFFECT_HAVE_BONUS;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_EFFECT_HAVE_BONUS;
		break;
	}
	default:
		break;
	}
}

void PlayerLevelDownTransformState::Update(int dt)
{
}

void PlayerLevelDownTransformState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	bool isLastFrame = animation->GetIsLastFrame();

	if (isLastFrame) {
		animation->ResetAnimation();
		mario->SetLevel(mario->GetLevel() - 1);
		mario->StartHideAndUnhide();
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

PlayerState* PlayerLevelDownTransformState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerLevelDownTransformState();
	SetAnimation();
	return __instance;
}

PlayerLevelDownTransformState::~PlayerLevelDownTransformState()
{
}
