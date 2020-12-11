#include "PlayerSpinningState.h"
#include "PlayerFallingSlowlyState.h"

PlayerState* PlayerSpinningState::__instance = NULL;
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
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	LPANIMATION animation = animationDatabase->Get(animationID);
	bool isLastFrame = animation->GetIsLastFrame();
	int currentFrame = animation->GetCurrentFrame();
	if (mario->nx > 0) {
		if (currentFrame == 1 || currentFrame == 2 || currentFrame == 3) {
			offset = 0;
		}
		else {
			offset = 6;
		}
	}
	else if (mario->nx < 0) {
		if (currentFrame == 2) {
			offset = -6;
		}
		else {
			offset = 0;
		}
	}
	if (currentFrame == 0 || currentFrame == 4) {
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
	else if (currentFrame == 2) {
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
	/*else {
		tailFront->stillAlive = false;
		tailBack->stillAlive = false;
	}*/
	//else if(currentFrame == 4){}
	if (isLastFrame) {
		animation->ResetAnimation();
		tailFront->stillAlive = false;
		tailBack->stillAlive = false;
		mario->SetIsSpinning(false);
		if(game->IsKeyDown(DIK_A))
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
		return __instance;
}

PlayerSpinningState::~PlayerSpinningState()
{
}
