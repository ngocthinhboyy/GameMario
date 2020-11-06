#include "PlayerKickingState.h"
#include "Mario.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "Debug.h"

PlayerState* PlayerKickingState::__instance = NULL;
PlayerKickingState::PlayerKickingState()
{
}

void PlayerKickingState::SetAnimation(int levelPlayer)
{
	Mario* mario = Mario::GetInstance();
	switch (levelPlayer) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_KICK;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_KICK;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_KICK;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_KICK;
		break;
	}
	default:
		break;
	}
}

void PlayerKickingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
}

void PlayerKickingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	SetAnimation(mario->GetLevel());
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	DebugOut(L"CURRRENTTt %d\n", animation->GetCurrentFrame());
	bool isLastFrame = animation->GetIsLastFrame();
	if (isLastFrame) {
		animation->ResetAnimation();
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

PlayerState* PlayerKickingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerKickingState();
	return __instance;
}

PlayerKickingState::~PlayerKickingState()
{
}
