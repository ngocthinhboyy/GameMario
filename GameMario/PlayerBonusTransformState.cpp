#include "PlayerBonusTransformState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "PlayerFallingState.h"
#include "PlayerFallingSlowlyState.h"

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
	if ((animationID == MARIO_EFFECT_GROW_UP && GetTickCount64() - timeStartTransform >= 810) || (animationID == MARIO_EFFECT_HAVE_BONUS && GetTickCount64() - timeStartTransform >= 610)) {
		mario->SetLevel(mario->GetLevel() + 1);
		countTransform = 0;
		stateWhenGrowingUp = STATE_MARIO_SMALL;
		Mario::GetInstance()->SetIsGrowingUp(false);
		if (changeCrouching) {
			mario->SetIsCrouching(false);
			mario->y -= MARIO_DEVIATION_CROUCHING_Y;
		}
		changeCrouching = false;
		if (mario->GetIsOnGround())
			mario->ChangeState(PlayerStandingState::GetInstance());
		else {
			if(mario->GetLevel() == MARIO_LEVEL_RACCOON)
				mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
			else
				mario->ChangeState(PlayerFallingState::GetInstance());
		}
	}
	else if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
		DWORD now = GetTickCount64();
		if (now - startTransform >= 80) {
			if (stateWhenGrowingUp == STATE_MARIO_SMALL) {
				stateWhenGrowingUp = STATE_MARIO_MIDDLE;
				mario->y -= 12;
				mario->h = 57;
				startTransform = now;
				countTransform++;
			}
			else if (stateWhenGrowingUp == STATE_MARIO_MIDDLE) {
				if (countTransform < 5) {
					stateWhenGrowingUp = STATE_MARIO_SMALL;
					mario->y += 12;
					mario->h = 45;
					startTransform = now;
					countTransform++;
				}
				else {
					stateWhenGrowingUp = STATE_MARIO_BIG;
					mario->y -= 24;
					mario->h = 81;
					startTransform = now;
					countTransform++;
				}
			}
			else if (stateWhenGrowingUp == STATE_MARIO_BIG) {
				if (countTransform < 8) {
					mario->y += 24;
					mario->h = 57;
					stateWhenGrowingUp = STATE_MARIO_MIDDLE;
					startTransform = now;
					countTransform++;
				}
			}
		}
	}
}

void PlayerBonusTransformState::OnKeyUp(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_DOWN: {
		if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
			if (mario->GetIsCrouChing()) {
				changeCrouching = true;
			}
		}
		break;
	}
	default:
		break;
	}
}

DWORD PlayerBonusTransformState::startTransform = 0;
DWORD PlayerBonusTransformState::timeStartTransform = 0;
int PlayerBonusTransformState::stateWhenGrowingUp = STATE_MARIO_SMALL;
PlayerState* PlayerBonusTransformState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerBonusTransformState();
	if (Mario::GetInstance()->GetIsCrouChing())
		Mario::GetInstance()->y -= MARIO_DEVIATION_CROUCHING_Y;
	if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_SMALL) {
		stateWhenGrowingUp = STATE_MARIO_SMALL;
		Mario::GetInstance()->SetIsGrowingUp(true);
	}
	AnimationDatabase::GetInstance()->Get(animationID)->ResetAnimation();
	SetAnimation();
	startTransform = GetTickCount64();
	timeStartTransform = GetTickCount64();
	return __instance;
}

PlayerBonusTransformState::~PlayerBonusTransformState()
{
}
