#include "PlayerSpinningState.h"

PlayerState* PlayerSpinningState::__instance = NULL;
GameObject* PlayerSpinningState::tailFront = NULL;
GameObject* PlayerSpinningState::tailBack = NULL;
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
	if (isLastFrame) {
		animation->ResetAnimation();
		tailFront->stillAlive = false;
		tailBack->stillAlive = false;
		if(game->IsKeyDown(DIK_A))
			mario->ChangeState(PlayerRunningState::GetInstance());
		else if (mario->vy > 0) {
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
	mario->vx = 0;
	if (tailFront == NULL && tailBack == NULL) {
			tailFront = new Tail(mario->x + MARIO_BIG_BBOX_WIDTH, mario->y + MARIO_BIG_BBOX_HEIGHT / 2, 20, 30);
			tailBack = new Tail(mario->x - 20, mario->y + MARIO_BIG_BBOX_HEIGHT / 2, 20, 30);

			tailFront->stillAlive = true;
			tailBack->stillAlive = true;

			Grid::GetInstance()->DeterminedGridToObtainObject(tailFront);
			Grid::GetInstance()->DeterminedGridToObtainObject(tailBack);
	}
	else if(tailFront !=NULL && tailBack !=NULL) {
		tailFront->SetPosition(mario->x + MARIO_BIG_BBOX_WIDTH, mario->y + MARIO_BIG_BBOX_HEIGHT / 2);
		tailBack->SetPosition(mario->x - 20, mario->y + MARIO_BIG_BBOX_HEIGHT / 2);

		tailFront->stillAlive = true;
		tailBack->stillAlive = true;

		Grid::GetInstance()->DeterminedGridToObtainObject(tailFront);
		Grid::GetInstance()->DeterminedGridToObtainObject(tailBack);
	}
		return __instance;
}

PlayerSpinningState::~PlayerSpinningState()
{
}
