#include "PlayerThrowingFireballState.h"

PlayerState* PlayerThrowingFireballState::__instance = NULL;
int PlayerThrowingFireballState::quantityFireball = 0;

PlayerThrowingFireballState::PlayerThrowingFireballState()
{
}

void PlayerThrowingFireballState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel())
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
	DWORD now = GetTickCount64();
	if (now - lastThrowingTime < FIREBALL_TIME_LIMIT_TO_THROWING && quantityFireball == FIREBALL_MAX_QUANTITY) {
		lastThrowingTime = now;
		mario->ChangeState(PlayerStandingState::GetInstance());
		return;
	};
	lastThrowingTime = now;
}

void PlayerThrowingFireballState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	DWORD now = GetTickCount64();
	if (now - lastThrowingTime < FIREBALL_TIME_LIMIT_TO_THROWING && quantityFireball == FIREBALL_MAX_QUANTITY) {
		lastThrowingTime = now;
		animation->ResetAnimation();
		mario->ChangeState(PlayerStandingState::GetInstance());
		return;
	};
	lastThrowingTime = now;
	bool isLastFrame = animation->GetIsLastFrame();
	if (isLastFrame) {
		if (now - lastThrowingTime < FIREBALL_TIME_LIMIT_TO_THROWING && quantityFireball < FIREBALL_MAX_QUANTITY) {
			PlayScene* scene = dynamic_cast<PlayScene*> (game->GetCurrentScene());
			Fireball* fireball = new Fireball(mario->x + MARIO_BIG_BBOX_WIDTH / 2, mario->y - 10, FIREBALL_WIDTH, FIREBALL_HEIGHT, FIREBALL_TYPE_WEAPON);
			Grid* grid = Grid::GetInstance();
			grid->DeterminedGridToObtainObject(fireball);
			quantityFireball++;
		}
		animation->ResetAnimation();
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

PlayerState* PlayerThrowingFireballState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerThrowingFireballState();
	SetAnimation();
	return __instance;
}

PlayerThrowingFireballState::~PlayerThrowingFireballState()
{
}
