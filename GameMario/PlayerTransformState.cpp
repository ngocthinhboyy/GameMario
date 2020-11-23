#include "PlayerTransformState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "debug.h"

PlayerState* PlayerTransformState::__instance = NULL;
PlayerTransformState::PlayerTransformState()
{
}

void PlayerTransformState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()) {
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_EFFECT_HAVE_BONUS;
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

void PlayerTransformState::Update(int dt)
{
}

void PlayerTransformState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	bool isLastFrame = animation->GetIsLastFrame();

	if (isLastFrame) {
		animation->ResetAnimation();
		mario->SetLevel(mario->GetLevel() + 1);
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

PlayerState* PlayerTransformState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerTransformState();
	SetAnimation();
	return __instance;
}

PlayerTransformState::~PlayerTransformState()
{
}
