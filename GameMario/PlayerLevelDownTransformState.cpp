#include "PlayerLevelDownTransformState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "debug.h"


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
		animationID = MARIO_EFFECT_LEVEL_DOWN_FROM_LEVEL_BIG;
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
DWORD PlayerLevelDownTransformState::startTransform = 0;
int PlayerLevelDownTransformState::stateWhenGrowingUp = STATE_MARIO_BIG;
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
		countTransform = 0;
		mario->StartHideAndUnhide();
		stateWhenGrowingUp = STATE_MARIO_BIG;
		Mario::GetInstance()->SetIsGrowingUp(false);
		mario->ChangeState(PlayerStandingState::GetInstance());

	} else if (mario->GetLevel() == MARIO_LEVEL_BIG) {
		DWORD now = GetTickCount64();
		if (now - startTransform >= 80) {
			if (stateWhenGrowingUp == STATE_MARIO_BIG) {
				stateWhenGrowingUp = STATE_MARIO_MIDDLE;
				mario->y += 24;
				mario->h = 57;
				startTransform = now;
				countTransform++;
			}
			else if (stateWhenGrowingUp == STATE_MARIO_MIDDLE) {
				if (countTransform < 5) {
					stateWhenGrowingUp = STATE_MARIO_BIG;
					mario->y -= 24;
					mario->h = 81;
					startTransform = now;
					countTransform++;
				}
				else {
					stateWhenGrowingUp = STATE_MARIO_SMALL;
					mario->y += 12;
					mario->h = 45;
					startTransform = now;
					countTransform++;
				}
			}
			else if (stateWhenGrowingUp == STATE_MARIO_SMALL) {
				if (countTransform < 8) {
					mario->y -= 12;
					mario->h = 57;
					stateWhenGrowingUp = STATE_MARIO_MIDDLE;
					startTransform = now;
					countTransform++;
				}
			}
		}
	}
}

PlayerState* PlayerLevelDownTransformState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerLevelDownTransformState();
	if (Mario::GetInstance()->GetIsCrouChing())
		Mario::GetInstance()->y -= MARIO_DEVIATION_CROUCHING_Y;
	if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_BIG) {
		stateWhenGrowingUp = STATE_MARIO_BIG;
		Mario::GetInstance()->SetIsGrowingUp(true);
	}
	startTransform = GetTickCount64();
	SetAnimation();
	return __instance;
}

PlayerLevelDownTransformState::~PlayerLevelDownTransformState()
{
}
