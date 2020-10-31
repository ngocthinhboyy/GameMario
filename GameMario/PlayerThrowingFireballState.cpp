#include "PlayerThrowingFireballState.h"
#include "Mario.h"
#include "game.h"
#include "AnimationDatabase.h"
#include "PlayerStandingState.h"
#include "Fireball.h"
#include "PlayScene.h"

PlayerState* PlayerThrowingFireballState::__instance = NULL;
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
	SetAnimation(mario->GetLevel());
}

void PlayerThrowingFireballState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	PlayScene* scene = dynamic_cast<PlayScene*> (game->GetCurrentScene());
	Fireball* fireball = new Fireball(mario->x + 2, mario->y, 15, 15);
	scene->objects.push_back(fireball);
	bool isLastFrame = animation->GetIsLastFrame();
	fireball->Render();
	if (isLastFrame) {
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
