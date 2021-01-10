#include "PlayerHoldingState.h"
#include "Mario.h"
#include "game.h"
#include "PlayerStandingState.h"
#include "debug.h"
#include "PlayScene.h"
#include "EnemyDefine.h"
#include "PlayerKickingState.h"

PlayerState* PlayerHoldingState::__instance = NULL;
bool PlayerHoldingState::isStanding = false;
bool PlayerHoldingState::stopIncreasingSpeed = true;
bool PlayerHoldingState::alreadyMaxJumping = false;

PlayerHoldingState::PlayerHoldingState()
{
}

void PlayerHoldingState::SetAnimation()
{
	Mario* mario = Mario::GetInstance();
	switch (mario->GetLevel()){
	case MARIO_LEVEL_BIG:
	{
		if (isStanding) {
			animationID = MARIO_ANI_BIG_HOLD_IDLE;
		}
		else {
			animationID = MARIO_ANI_BIG_HOLD;
		}
		break;
	}
	case MARIO_LEVEL_SMALL:
	{
		if (isStanding) {
			animationID = MARIO_ANI_SMALL_HOLD_IDLE;
		}
		else {
			animationID = MARIO_ANI_SMALL_HOLD;
		}
		break;
	}
	case MARIO_LEVEL_RACCOON:
	{
		if (isStanding) {
			animationID = MARIO_ANI_RACCOON_HOLD_IDLE;
		}
		else {
			animationID = MARIO_ANI_RACCOON_HOLD;
		}
		break;
	}
	case MARIO_LEVEL_FIRE:
	{
		if (isStanding) {
			animationID = MARIO_ANI_FIRE_HOLD_IDLE;
		}
		else {
			animationID = MARIO_ANI_FIRE_HOLD;
		}
		break;
	}
	default:
		break;
	}
}

void PlayerHoldingState::Update(int dt)
{
	Mario* mario = Mario::GetInstance();
	if (!stopIncreasingSpeed && !alreadyMaxJumping) {
		mario->vy += (dt * -MARIO_ACCELERATION_JUMP_Y);
	}
	if (!isStanding) {
		if ((abs(mario->vx) < MARIO_RUNNING_MAX_SPEED) && !isMaxSpeed && increaseSpeed) {
			mario->vx += (dt * MARIO_SPEED_ACCELERATION * (mario->nx));
		}
		if (!increaseSpeed && mario->vx != 0) {
			mario->vx += (dt * MARIO_SPEED_ACCELERATION * -(mario->nx));
		}
		if (mario->vx * mario->nx <= 0) {
			mario->vx = 0;
		}
		PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT> enemies = scene->enemies;
		Koopa* koopaRRR = NULL;
		for (LPGAMEOBJECT object : enemies) {
			if (koopaRRR = dynamic_cast<Koopa*>(object)) {
				if (koopaRRR->GetIsHold() && increaseSpeed)
				{
					koopaRRR->x += (mario->vx - koopaRRR->vx) * dt;
					break;
				}
			}
		}
	}
}

void PlayerHoldingState::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	Game* game = Game::GetInstance();
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
		if (mario->vx == 0) {
			isStanding = true;
			SetAnimation();
		}
		if (abs(mario->vx) >= MARIO_RUNNING_MAX_SPEED) {
			isMaxSpeed = true;
		}
		else {
			isMaxSpeed = false;
		}
		if (game->IsKeyDown(DIK_S)) {
			if(mario->vy == 0)
				stopIncreasingSpeed = false;
			if (abs(mario->vy) >= MARIO_JUMP_MAX_SPEED_Y_HOLDING) {
				alreadyMaxJumping = true;
			}
		}
		if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
			if (mario->vx != 0) {
				increaseSpeed = false;
			}
			else {
				isStanding = true;
			}
		}
		else if (game->IsKeyDown(DIK_RIGHT)) {
			isStanding = false;
			SetAnimation();
			increaseSpeed = true;
			mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			isStanding = false;
			SetAnimation();
			increaseSpeed = true;
			mario->nx = -1;
		}
		else {
			if (mario->vx != 0) {
				increaseSpeed = false;
			}
			else {
				isStanding = true;
			}
		}
	}
	else {
		if (koopa != NULL) {
			koopa->x += KOOPA_KICKING_AFTER_HOLDING_DISTANCE_X * (mario->nx);
			koopa->y += KOOPA_KICKING_AFTER_HOLDING_DISTANCE_Y;
			koopa->SetIsHold(false);
			koopa->noCollisionConsideration = false;
			isMaxSpeed = false;
			increaseSpeed = true;
			stopIncreasingSpeed = true;
			alreadyMaxJumping = false;
			mario->ChangeState(PlayerKickingState::GetInstance());
			mario->SetIsHolding(false);
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

void PlayerHoldingState::OnKeyUp(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{

	case DIK_S: {
		stopIncreasingSpeed = true;
		alreadyMaxJumping = false;
		break;
	}
	default:
		break;
	}
}

PlayerState* PlayerHoldingState::GetInstance()
{
	if (__instance == NULL) __instance = new PlayerHoldingState();
	Mario* mario = Mario::GetInstance();
	mario->SetIsRunning(false);
	mario->SetIsHolding(true);
	stopIncreasingSpeed = true;
	alreadyMaxJumping = false;
	SetAnimation();
	return __instance;
}

PlayerHoldingState::~PlayerHoldingState()
{
}
