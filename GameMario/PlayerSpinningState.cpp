#include "PlayerSpinningState.h"
#include "PlayerFallingSlowlyState.h"

PlayerState* PlayerSpinningState::__instance = NULL;
DWORD PlayerSpinningState::timeStartSpinning = 0;
Tail* PlayerSpinningState::tailFront = NULL;
Tail* PlayerSpinningState::tailBack = NULL;
int PlayerSpinningState::offset = 0;
PlayerSpinningState::PlayerSpinningState()
{
}

void PlayerSpinningState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()){
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_SPIN;
	}
	default:
		break;
	}
}

void PlayerSpinningState::Update(int dt)
{
}

void PlayerSpinningState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	DWORD now = GetTickCount64();
	if (mario->nx > 0) {
		if (now-timeStartSpinning >= 100 && now - timeStartSpinning < 200) {
			offset = 0;
		}
		else {
			offset = 6;
		}
	}
	else if (mario->nx < 0) {
		if (now - timeStartSpinning >= 150 && now - timeStartSpinning < 200) {
			offset = -6;
		}
		else {
			offset = 0;
		}
	}
	if ((now - timeStartSpinning >= 0 && now - timeStartSpinning < 50 )||  now - timeStartSpinning >= 250) {
		if (mario->nx > 0) {
			offset = 7;
			tailBack->stillAlive = true;
			tailFront->stillAlive = false;
		}
		else if (mario->nx < 0) {
			tailBack->stillAlive = false;
			tailFront->stillAlive = true;
		}
	}
	else if (now - timeStartSpinning >= 150 && now - timeStartSpinning < 200) {
		if (mario->nx > 0) {
			tailFront->stillAlive = true;
			tailBack->stillAlive = false;
		}
		else {
			tailFront->stillAlive = false;
			tailBack->stillAlive = true;
		}
	}
	else {
		tailFront->stillAlive = false;
		tailBack->stillAlive = false;
	}
	if (now - timeStartSpinning >= 250) {
		tailFront->stillAlive = false;
		tailBack->stillAlive = false;
		mario->SetIsSpinning(false);
		if (game->IsKeyDown(DIK_A))
			mario->ChangeState(PlayerRunningState::GetInstance());
		else if (mario->vy > 0) {
			if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
				mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
				return;
			}
			mario->ChangeState(PlayerFallingState::GetInstance());
		}
		else
			mario->ChangeState(PlayerStandingState::GetInstance());
	}
}

PlayerState* PlayerSpinningState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerSpinningState();
	AnimationDatabase::GetInstance()->Get(animationID)->ResetAnimation();
	SetAnimation();
	Mario* mario = Mario::GetInstance();
	mario->SetIsSpinning(true);
	mario->vx = 0;
	if (tailFront == NULL && tailBack == NULL) {
			tailFront = new Tail(mario->x + MARIO_BIG_BBOX_WIDTH, mario->y + MARIO_BIG_BBOX_HEIGHT / 2, 27, 30);
			tailBack = new Tail(mario->x - 27, mario->y + MARIO_BIG_BBOX_HEIGHT / 2, 27, 30);

			tailFront->stillAlive = false;
			tailBack->stillAlive = false;

			Grid::GetInstance()->DeterminedGridToObtainObject(tailFront);
			Grid::GetInstance()->DeterminedGridToObtainObject(tailBack);
	}
	else if(tailFront !=NULL && tailBack !=NULL) {
		tailFront->SetPosition(mario->x + MARIO_BIG_BBOX_WIDTH, mario->y + MARIO_BIG_BBOX_HEIGHT / 2);
		tailBack->SetPosition(mario->x - 27, mario->y + MARIO_BIG_BBOX_HEIGHT / 2);
		tailFront->SetHasEffect(false);
		tailBack->SetHasEffect(false);
		tailFront->noCollisionConsideration = false;
		tailBack->noCollisionConsideration = false;

		tailFront->stillAlive = false;
		tailBack->stillAlive = false;

		Grid::GetInstance()->DeterminedGridToObtainObject(tailFront);
		Grid::GetInstance()->DeterminedGridToObtainObject(tailBack);
	}
	timeStartSpinning = GetTickCount64();
	return __instance;
}

PlayerSpinningState::~PlayerSpinningState()
{
}
