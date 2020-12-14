#include "PlayerInWorldMapState.h"
#include "Mario.h"
#include "PlayerSpinningState.h"
#include "PlayScene.h"

PlayerInWorldMapState::PlayerInWorldMapState()
{
}

void PlayerInWorldMapState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()) {
	case MARIO_LEVEL_BIG:
	{
		animationID = 218;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = 118;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = 322;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = 419;
		break;
	}
	default:
		break;
	}
}

void PlayerInWorldMapState::Update(int dt)
{
}

void PlayerInWorldMapState::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
		case DIK_RIGHT: {
			if (directionX == 2 || directionX == 1) {
				Mario::GetInstance()->vx = 0.3f;
			}
			break;
		}
		case DIK_LEFT: {
			if (directionX == 2 || directionX == -1)
				Mario::GetInstance()->vx = -0.3f;
			break;
		}
		case DIK_UP: {
			if (directionY == 2 || directionY == -1)
				Mario::GetInstance()->vy = -0.3f;
			break;
		}
		case DIK_DOWN: {
			if (directionY == 2 || directionY == 1)
				Mario::GetInstance()->vy = 0.3f;
			break;
		}
		case DIK_S: {
			DWORD timeStart = GetTickCount64();
			Game::GetInstance()->SwitchScene(2);
			Mario::GetInstance()->nx = 1;
			PlayScene* playScene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			playScene->SetTimeOfPreviousSecond(timeStart);
			break;
		}
		case DIK_ESCAPE: {
			Game::GetInstance()->SwitchScene(3);
			break;
		}
		default:
			break;
	}
}

void PlayerInWorldMapState::KeyState(BYTE* states)
{
}

PlayerState* PlayerInWorldMapState::__instance = NULL;
int PlayerInWorldMapState::directionX = 0;
int PlayerInWorldMapState::directionY = 0;
int PlayerInWorldMapState::idGateMarioStandingOn = 0;

PlayerState* PlayerInWorldMapState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerInWorldMapState();
	SetAnimation();
	directionX = 0;
	directionY = 0;
	idGateMarioStandingOn = 0;
	return __instance;
}

PlayerInWorldMapState::~PlayerInWorldMapState()
{
}
