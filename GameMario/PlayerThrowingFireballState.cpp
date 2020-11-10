#include "PlayerThrowingFireballState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "Fireball.h"
#include "PlayScene.h"
#include "DynamicObjectDefine.h"

PlayerState* PlayerThrowingFireballState::__instance = NULL;
int PlayerThrowingFireballState::quantityFireball = 0;
PlayerThrowingFireballState::PlayerThrowingFireballState()
{
}

void PlayerThrowingFireballState::SetAnimation(int levelPlayer)
{
	switch (levelPlayer)
	{
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_THROW_FIREBALL;
	}
	default:
		break;
	}
}

void PlayerThrowingFireballState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	DWORD now = GetTickCount();
	if (now - lastThrowingTime < FIREBALL_TIME_LIMIT_TO_THROWING && quantityFireball == FIREBALL_MAX_QUANTITY) {
		lastThrowingTime = now;
		mario->ChangeState(PlayerStandingState::GetInstance());
		return;
	};
	lastThrowingTime = now;
	SetAnimation(mario->GetLevel());
}

void PlayerThrowingFireballState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	DWORD now = GetTickCount();
	if (now - lastThrowingTime < FIREBALL_TIME_LIMIT_TO_THROWING && quantityFireball == FIREBALL_MAX_QUANTITY) {
		lastThrowingTime = now;
		animation->ResetAnimation();
		mario->ChangeState(PlayerStandingState::GetInstance());
		return;
	};
	lastThrowingTime = now;
	bool isLastFrame = animation->GetIsLastFrame();
	if (isLastFrame) {
		if (quantityFireball < FIREBALL_MAX_QUANTITY) {
			PlayScene* scene = dynamic_cast<PlayScene*> (game->GetCurrentScene());
			Fireball* fireball = new Fireball(mario->x + MARIO_BIG_BBOX_WIDTH / 2, mario->y, FIREBALL_WIDTH, FIREBALL_HEIGHT);
			scene->objects.push_back(fireball);
			quantityFireball++;
		}
		animation->ResetAnimation();
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

PlayerState* PlayerThrowingFireballState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerThrowingFireballState();
	return __instance;
}

PlayerThrowingFireballState::~PlayerThrowingFireballState()
{
}
