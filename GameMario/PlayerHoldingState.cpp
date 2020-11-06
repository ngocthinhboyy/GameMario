#include "PlayerHoldingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "PlayScene.h"
#include "EnemyDefine.h"
#include "PlayerKickingState.h"

PlayerState* PlayerHoldingState::__instance = NULL;

PlayerHoldingState::PlayerHoldingState()
{
}

void PlayerHoldingState::SetAnimation(int levelPlayer)
{
	Mario* mario = Mario::GetInstance();
	switch (levelPlayer) {
	case MARIO_LEVEL_BIG:
	{
		animationID = MARIO_ANI_BIG_HOLD;
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		animationID = MARIO_ANI_SMALL_HOLD;
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		animationID = MARIO_ANI_RACCOON_HOLD;
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		animationID = MARIO_ANI_FIRE_HOLD;
		break;
	}
	default:
		break;
	}
}

void PlayerHoldingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if ((abs(mario->vx) < MARIO_RUNNING_MAX_SPEED) && !isMaxSpeed && increaseSpeed) {
		mario->vx += (dt * MARIO_SPEED_ACCELERATION * (mario->nx));
	}
	if (!increaseSpeed && mario->vx != 0) {
		mario->vx += (dt * MARIO_SPEED_ACCELERATION * -(mario->nx));
	}
	if (mario->vx * mario->nx <= 0) {
		mario->vx = 0;
	}
	/*if (isTurnBack) {
		mario->vx += (dt * MARIO_SPEED_ACCELERATION * 2.5 * -(mario->nx));
		if ((mario->vx * mario->nx) <= 0) {
			mario->nx = -mario->nx;
			if (koopa != NULL) {
				koopa->x = mario->x + 34.985596 * (mario->nx);
			}
			prevKeyIsLeft = false;
			prevKeyIsRight = false;
			isTurnBack = false;
		}
		return;
	}*/
	/*if ((mario->vx * mario->nx) <= 0) {
		mario->vx = 0;
		koopa->vx = mario->vx;
		koopa->vy = mario->vy;
	}*/
}

void PlayerHoldingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
	SetAnimation(mario->GetLevel());
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	vector<LPGAMEOBJECT> enemies = scene->enemies;
	Koopa* koopa = NULL;
	for (LPGAMEOBJECT object : enemies) {
		if (koopa = dynamic_cast<Koopa*>(object)) {
			if (koopa->GetIsHold())
				break;
		}
	}
	if (game->IsKeyDown(DIK_A)) {
		if (abs(mario->vx) >= MARIO_RUNNING_MAX_SPEED) {
			isMaxSpeed = true;
		}
		else {
			isMaxSpeed = false;
		}
		if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
			if (mario->vx != 0) {
				increaseSpeed = false;
			}
		}
		else if (game->IsKeyDown(DIK_RIGHT)) {
				increaseSpeed = true;
				mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
				increaseSpeed = true;
				mario->nx = -1;
		}
		else {
			if (mario->vx != 0) {
				increaseSpeed = false;
			}
		}
	}
	else {
		if (koopa != NULL) {
			koopa->y += 7;
			koopa->x += 10 * (mario->nx);
			koopa->SetIsHold(false);
			isMaxSpeed = false;
			increaseSpeed = true;
			mario->ChangeState(PlayerKickingState::GetInstance());
			if (mario->nx > 0) {
				koopa->vx = KOOPA_SPEED_TORTOISESHELL;
				koopa->SetState(ENEMY_STATE_SPIN_DIE_KICK);
			}
			else {
				koopa->vx = -KOOPA_SPEED_TORTOISESHELL;
				koopa->SetState(ENEMY_STATE_SPIN_DIE_KICK);
			}
		}
	}
}

void PlayerHoldingState::OnKeyDown(int KeyCode)
{
}

PlayerState* PlayerHoldingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerHoldingState();
	Mario* mario = Mario::GetInstance();
	mario->SetIsRunning(false);
	return __instance;
}

PlayerHoldingState::~PlayerHoldingState()
{
}
