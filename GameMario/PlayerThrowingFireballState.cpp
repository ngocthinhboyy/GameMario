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
	DWORD now = GetTickCount64();
	if (now - lastThrowingTime < FIREBALL_TIME_LIMIT_TO_THROWING && quantityFireball == FIREBALL_MAX_QUANTITY) {
		lastThrowingTime = now;
		mario->ChangeState(PlayerStandingState::GetInstance());
		return;
	};
	lastThrowingTime = now;
	if (now - timeStartThrowing >= 160) {
		if (now - lastThrowingTime < FIREBALL_TIME_LIMIT_TO_THROWING && quantityFireball < FIREBALL_MAX_QUANTITY) {
			PlayScene* scene = dynamic_cast<PlayScene*> (game->GetCurrentScene());
			Fireball* fireball = new Fireball(mario->x + MARIO_BIG_BBOX_WIDTH / 2, mario->y - 10, FIREBALL_WIDTH, FIREBALL_HEIGHT, FIREBALL_TYPE_WEAPON);
			Grid* grid = Grid::GetInstance();
			grid->DeterminedGridToObtainObject(fireball);
			quantityFireball++;
		}
		mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

DWORD PlayerThrowingFireballState::timeStartThrowing = 0;
PlayerState* PlayerThrowingFireballState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerThrowingFireballState();
	SetAnimation();
	AnimationDatabase::GetInstance()->Get(animationID)->ResetAnimation();
	timeStartThrowing = GetTickCount64();
	return __instance;
}

PlayerThrowingFireballState::~PlayerThrowingFireballState()
{
}
