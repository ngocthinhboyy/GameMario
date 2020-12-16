#include "Koopa.h"
#include "MarioDefine.h"
#include "PlayScene.h"
#include "EnemyDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "PlayerJumpingState.h"
#include "PlayerKickingState.h"
#include "PlayerHoldingState.h"
#include "Camera.h"
#include "QuestionBrick.h"
#include "StaticObjectDefine.h"
#include "Leaf.h"
#include "ItemDefine.h"
#include "Grid.h"
#include "Coin.h"
#include "PlayerLevelDownTransformState.h"
#include "PlayerStandingState.h"
#include "Point.h"
#include "EspecialBrick.h"
#include "FragmentOfEspecialBrick.h"
#include "PlayerDieState.h"
#include "Flower.h"
#include "Fireball.h"
#include "Mushroom.h"

Koopa::Koopa()
{
}

Koopa::Koopa(float x, float y, float w, float h, int typeKoopa, int typeMove)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = typeKoopa;
	this->gameObjectID = idGenerate++;
	this->nx = -1;
	this->startNx = -1;
	this->startPositionX = x;
	this->startPositionY = y;
	this->startTypeMove = typeMove;
	this->vx = -KOOPA_WALKING_SPEED_X;
	if(typeMove == 1)
		SetState(ENEMY_STATE_WALKING);
	else if (typeMove == 2) {

		SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
	}
}

void Koopa::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	if (vx > 0)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if(isUpsideDown)
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_FLIP_SCALE);
	if (animation != NULL) {
		if (state == ENEMY_STATE_DIE || state == ENEMY_STATE_SPIN_DIE_KICK) {
			animation->Render(x, y, alpha, scale);
		}
		else {
			animation->Render(x, y - 31, alpha, scale);
		}
	}
	//RenderBoundingBox();
}

void Koopa::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	if (type == 1) {
		switch (state)
		{
		case ENEMY_STATE_WALKING:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_WALKING);
			break;
		}
		case ENEMY_STATE_DIE:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_DIE);
			break;
		}
		case ENEMY_STATE_SPIN_DIE_KICK:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_SPIN_DIE_KICK);
			break;
		}
		case ENEMY_STATE_WALKING_WITH_SWINGS:
		{
			animation = animationDatabase->Get(KOOPA_RED_ANI_WALKING_WITH_SWINGS);
			break;
		}
		default:
			break;
		}
	}
	else if (type == 2) {
		switch (state)
		{
		case ENEMY_STATE_WALKING:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_WALKING);
			break;
		}
		case ENEMY_STATE_DIE:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_DIE);
			break;
		}
		case ENEMY_STATE_SPIN_DIE_KICK:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_SPIN_DIE_KICK);
			break;
		}
		case ENEMY_STATE_WALKING_WITH_SWINGS:
		{
			animation = animationDatabase->Get(KOOPA_GREEN_ANI_WALKING_WITH_SWINGS);
			break;
		}
		default:
			break;
		}
	}
}

void Koopa::Update(DWORD dt, int scaleTime)
{
	if (state == ENEMY_STATE_DIE && !isDiedByFireball) {
		if (GetTickCount64() - timeDie >= 9000) {
			this->vx = -KOOPA_WALKING_SPEED_X * Mario::GetInstance()->nx;
			this->nx = -Mario::GetInstance()->nx;
			this->isUpsideDown = false;
			this->isDiedByFireball = false;
			this->noCollisionConsideration = false;
			y -= 40;
			if (startTypeMove == 1) {
				SetState(ENEMY_STATE_WALKING);
			}
			else if (startTypeMove == 2) {

				SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
			}
			if (isHold){
				isHold = false;
				this->vx = KOOPA_WALKING_SPEED_X * Mario::GetInstance()->nx;
				this->nx = Mario::GetInstance()->nx;
				Mario::GetInstance()->SetIsHolding(false);
				Mario::GetInstance()->ChangeState(PlayerStandingState::GetInstance());
			}
		}
	}
	if (isHold) {
		Mario* mario = Mario::GetInstance();
		if (Mario::GetInstance()->nx * vx <= 0) {
			x = mario->x + KOOPA_HOLDING_DISTANCE_TURN_BACK_X * (mario->nx);
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				y = mario->y - KOOPA_HOLDING_DISTANCE_TURN_BACK_Y_MARIO_SMALL;
			else
				y = mario->y - KOOPA_HOLDING_DISTANCE_TURN_BACK_Y;
		}
		/*if (!mario->GetIsCollisionWithWall()) {
			vx = mario->vx;
			vy = mario->vy;
		}
		else {
			vx = 0;
			vy = 0;
		}*/
	}
	else {
		vy += ENEMY_GRAVITY * dt;
	}
	GameObject::Update(dt, scaleTime);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjs = scene->objects;
	vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	//for (LPGAMEOBJECT x : scene->enemies) {
	//	if (x->GetState() != ENEMY_STATE_DIE)
	//		coEnemies.push_back(x);
	//}
	if (isHold)
		CheckOverlapWithEnemy(coEnemies);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (stillAlive && !isDiedByFireball) {
		/*if (state == ENEMY_STATE_SPIN_DIE_KICK || isHold)
		{
			CalcPotentialCollisions(&coEnemies, coEvents);
		}*/
		CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
		CalcPotentialCollisions(&coObjs, coEvents);
		CalcPotentialCollisions(&coEnemies, coEvents);
	}

	if (coEvents.size() == 0 || !stillAlive)
	{
			x += dx;
			y += dy;
			float cam_x, cam_y;
			Camera* camera = Camera::GetInstance();
			camera->GetCamPos(cam_x, cam_y);
			if (y > (cam_y + SCREEN_HEIGHT) && isDiedByFireball)
				stillAlive = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;


		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		int coEventsSize = coEventsResult.size();

		for (UINT i = 0; i < coEventsSize; i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (LPCOLLISIONMAPOBJECT collMapObj = dynamic_cast<LPCOLLISIONMAPOBJECT> (e->obj)) {
				CollisionWithCollisionMapObject(e, collMapObj);
			}
			else if(LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
				if (e->ny != 0) {
					vy = 0;
					if (e->ny < 0)
						enemy->y -= 0.4f;
				}
				if (e->nx != 0) {
					if (state == ENEMY_STATE_SPIN_DIE_KICK) {
						if (Koopa * koopa = dynamic_cast<Koopa*> (enemy)) {
							koopa->SetState(ENEMY_STATE_DIE);
							koopa->SetIsDiedByFireball();
						}
						else if (Goomba * goomba = dynamic_cast<Goomba*> (enemy)) {
							if (goomba->GetType() == 2)
								goomba->SetState(ENEMY_STATE_WALKING);
						}
						else if (Flower * flower = dynamic_cast<Flower*> (enemy)) {
							flower->SetExplosiveDied(true);
							flower->SetTimeDie();
							flower->noCollisionConsideration = true;
						}
						else
							enemy->stillAlive = false;
						if (enemy->x > Mario::GetInstance()->x) {
							enemy->vx = ENEMY_DIE_SPEED_X;
						}
						else {
							enemy->vx = -ENEMY_DIE_SPEED_X;
						}
						enemy->vy = -ENEMY_DIE_SPEED_Y;
						enemy->SetIsUpsideDown(true);
						enemy->noCollisionConsideration = true;
					}
					else {
						if (enemy->state == ENEMY_STATE_SPIN_DIE_KICK) {
							state = ENEMY_STATE_DIE;
							isDiedByFireball = true;
							vy = -ENEMY_DIE_SPEED_Y;
							if (x > Mario::GetInstance()->x) {
								vx = ENEMY_DIE_SPEED_X;
							}
							else {
								vx = -ENEMY_DIE_SPEED_X;
							}
							isUpsideDown = true;
							noCollisionConsideration = true;
						}
						else {
							if (isHold) {
								if (Koopa* koopa = dynamic_cast<Koopa*> (enemy)) {
									koopa->SetState(ENEMY_STATE_DIE);
									koopa->SetIsDiedByFireball();
								}
								if (enemy->x > Mario::GetInstance()->x) {
									enemy->vx = ENEMY_DIE_SPEED_X;
									vx = ENEMY_DIE_SPEED_X;
								}
								else {
									enemy->vx = -ENEMY_DIE_SPEED_X;
									vx = -ENEMY_DIE_SPEED_X;
								}
								enemy->vy = -ENEMY_DIE_SPEED_Y;
								enemy->SetIsUpsideDown(true);
								enemy->noCollisionConsideration = true;
								noCollisionConsideration = true;
								vy = -ENEMY_DIE_SPEED_Y * 1.2f;
								isHold = false;
								isUpsideDown = true;
								isDiedByFireball = true;
								Mario::GetInstance()->ChangeState(PlayerStandingState::GetInstance());
							}
							else {
								vx = -vx;
								enemy->vx = -enemy->vx;
							}
						}
					}
				}
			}
			else if (QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*> (e->obj)) {
				if (e->ny != 0) {
					vy = 0;
					if (this->state == ENEMY_STATE_WALKING_WITH_SWINGS) {
						vy -= KOOPA_WALKING_WITH_SWINGS_Y;
					}
				}
				if (e->nx != 0) {
					if (state == ENEMY_STATE_SPIN_DIE_KICK) {
						vx = -vx;
						if (!questionBrick->isEmptyBrick) {
							if (questionBrick->GetType() == QUESTION_BRICK_TYPE_HAS_ESPECIAL_ITEM) {
								if (Mario::GetInstance()->GetLevel() != MARIO_LEVEL_SMALL) {
									Leaf* leaf = new Leaf(questionBrick->x + QUESTION_BRICK_BBOX_WIDTH / 2 - LEAF_BBOX_WIDTH / 2, questionBrick->y - 3, LEAF_BBOX_WIDTH, LEAF_BBOX_HEIGHT);
									leaf->vy = -LEAF_SPEED_Y_APPEAR;
									leaf->vx = LEAF_SPEED;
									Grid::GetInstance()->DeterminedGridToObtainObject(leaf);
								}
							}
							else if (questionBrick->GetType() == QUESTION_BRICK_TYPE_HAS_COIN) {
								Coin* coin = new Coin(questionBrick->x + QUESTION_BRICK_BBOX_WIDTH / 2 - COIN_BBOX_WIDTH / 2, questionBrick->y - 3, COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT,1);
								coin->vy = -COIN_SPEED_Y;
								Grid::GetInstance()->DeterminedGridToObtainObject(coin);
							}
						}
						questionBrick->isEmptyBrick = true;
					}
					else {
						vx = -vx;
					}
				}
			}
			else if (EspecialBrick * especialBrick = dynamic_cast<EspecialBrick*> (e->obj)) {
				CollisionWithEspecialBrick(e, especialBrick);
			}
			else if (dynamic_cast<Coin*> (e->obj)) {
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
					y += dy;
			}
			else if (dynamic_cast<Fireball*> (e->obj)) {
				if (e->ny != 0) {
					if (e->ny > 0) {
						y -= dy;
					}
				}
			}
			else if (dynamic_cast<Mushroom*>(e->obj)) {
				if (e->ny != 0) y += dy;
				if (e->nx != 0) x += dx;
			}
			/*else {
				if (e->nx != 0) vx = -vx;
				if (e->ny != 0) vy = 0;
			}*/
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Koopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == ENEMY_STATE_DIE || state == ENEMY_STATE_SPIN_DIE_KICK) {
		/*l = x - KOOPA_BBOX_WIDTH / 2;
		t = y - KOOPA_DIE_BBOX_HEIGHT / 2;*/
		l = x;
		t = y;
		r = l + KOOPA_BBOX_WIDTH + 10;
		b = t + KOOPA_DIE_BBOX_HEIGHT;
	}
	else {
		/*l = x - KOOPA_BBOX_WIDTH / 2;
		t = y - KOOPA_BBOX_HEIGHT / 2;*/
		l = x;
		t = y;
		r = l + KOOPA_BBOX_WIDTH;
		b = t + KOOPA_BBOX_HEIGHT;
	}
}

void Koopa::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
{
	int collisionMapObjectDirectionX = collisionMapObject->GetCollisionDirectionX();
	int collisionMapObjectDirectionY = collisionMapObject->GetCollisionDirectionY();
	if (collisionEvent->nx != 0) {
		if (collisionMapObjectDirectionX == 0)
				x += dx;
		else if (collisionEvent->nx < 0 && collisionMapObjectDirectionX == 1)
				x += dx;
		else if (collisionEvent->nx > 0 && collisionMapObjectDirectionX == -1)
				x += dx;
		else
			vx = -vx;
	}
	if (collisionEvent->ny != 0) {
		if (state == ENEMY_STATE_WALKING) {
			if (x + dx <= collisionMapObject->x || x + dx + KOOPA_BBOX_WIDTH >= collisionMapObject->x + collisionMapObject->w) {
				vx = -vx;
			}
			if (x < collisionMapObject->x)
				x = collisionMapObject->x;
			else if (x + KOOPA_BBOX_WIDTH > collisionMapObject->x + collisionMapObject->w)
				x = collisionMapObject->x + collisionMapObject->w - KOOPA_BBOX_WIDTH;
		}
		if (collisionMapObjectDirectionY == 0)
		{
			y += dy;
		}
		else if (collisionEvent->ny < 0 && collisionMapObjectDirectionY == 1) {
			y += dy;
		}
		else if (collisionEvent->ny > 0 && collisionMapObjectDirectionY == -1) {
			y += dy;
		}
		else
		{
			vy = 0;
			if (this->state == ENEMY_STATE_WALKING_WITH_SWINGS) {
				vy -= KOOPA_WALKING_WITH_SWINGS_Y;
			}
			else if (this->state == ENEMY_STATE_DIE) {
				vx = 0;
			}
		}
	}
}

void Koopa::CollisionWithEspecialBrick(LPCOLLISIONEVENT collisionEvent, EspecialBrick* especialBrick)
{
	if (collisionEvent->nx != 0) {
		vx = -vx;
		if (state == ENEMY_STATE_SPIN_DIE_KICK) {
			if (especialBrick->stillAlive) {
				especialBrick->stillAlive = false;
				FragmentOfEspecialBrick* fragment1 = new FragmentOfEspecialBrick(especialBrick->x, especialBrick->y, 39, 39, -0.15, -1.1);
				FragmentOfEspecialBrick* fragment2 = new FragmentOfEspecialBrick(especialBrick->x, especialBrick->y, 39, 39, 0.15, -1.1);
				FragmentOfEspecialBrick* fragment3 = new FragmentOfEspecialBrick(especialBrick->x, especialBrick->y, 39, 39, -0.15, -0.8);
				FragmentOfEspecialBrick* fragment4 = new FragmentOfEspecialBrick(especialBrick->x, especialBrick->y, 39, 39, 0.15, -0.8);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment1);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment2);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment3);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment4);
			}
		}
	}
	if (collisionEvent->ny != 0) {
		vy = 0;
		if (collisionEvent->ny < 0) {
			if (state == ENEMY_STATE_WALKING) {
				if (x + dx <= especialBrick->x - 20 || x + dx + KOOPA_BBOX_WIDTH >= especialBrick->x + especialBrick->w + 20) {
					vx = -vx;
				}
				if (x < especialBrick->x - 20)
					x = especialBrick->x;
				else if (x + KOOPA_BBOX_WIDTH > especialBrick->x + especialBrick->w + 20)
					x = especialBrick->x + especialBrick->w - KOOPA_BBOX_WIDTH;
			}
		}
	}
}

void Koopa::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	if (!mario->GetUntouchable()) {
		if (collisionEvent->nx != 0) {
			if (!isHold) {
				mario->vx = 0;
				if (state == ENEMY_STATE_DIE) {
					if (mario->GetIsRunning()) {
						isHold = true;
						noCollisionConsideration = true;
						mario->ChangeState(PlayerHoldingState::GetInstance());
					}
					else {
						mario->ChangeState(PlayerKickingState::GetInstance());
						if (collisionEvent->nx > 0) {
							vx = -KOOPA_SPEED_TORTOISESHELL;
							state = ENEMY_STATE_SPIN_DIE_KICK;
						}
						else {
							vx = KOOPA_SPEED_TORTOISESHELL;
							state = ENEMY_STATE_SPIN_DIE_KICK;
						}
					}
				}
				else {
					if (mario->GetIsHolding()) {
						vector<LPGAMEOBJECT> enemies = scene->enemies;
						for (auto enemy : enemies) {
							if (Koopa* koopa = dynamic_cast<Koopa*> (enemy)) {
								if (koopa->GetIsHold()) {
									koopa->SetIsUpsideDown(false);
									koopa->noCollisionConsideration = false;
									koopa->y -= 40;
									koopa->SetState(ENEMY_STATE_WALKING);
									koopa->SetIsHold(false);
									koopa->vx = KOOPA_WALKING_SPEED_X * Mario::GetInstance()->nx;
									koopa->nx = Mario::GetInstance()->nx;
									Mario::GetInstance()->SetIsHolding(false);
								}
							}
						}
					}
					if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
						mario->StartUntouchable();
						mario->ChangeState(PlayerLevelDownTransformState::GetInstance());
						scene->StopGame(1000);
						mario->vx = 0;
						mario->vy = 0;
					}
					else if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_SMALL) {
						Mario::GetInstance()->ChangeState(PlayerDieState::GetInstance());
						PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
						Mario::GetInstance()->noCollisionConsideration = true;
						scene->StopGame(5000);
					}
				}
			}
		}
	}
	else {
		mario->x += mario->dx;
	}
	if (collisionEvent->ny != 0) {
		if (collisionEvent->ny < 0) {
			if (state != ENEMY_STATE_DIE) {
				if (state != ENEMY_STATE_WALKING_WITH_SWINGS) {
					Point* point = new Point(x, y, 39, 30, 100);
					Grid* grid = Grid::GetInstance();
					grid->DeterminedGridToObtainObject(point);
					mario->SetPoint(mario->GetPoint() + 100);
					state = ENEMY_STATE_DIE;
					mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
					vx = 0;
					SetTimeDie();
				}
				else if (state == ENEMY_STATE_WALKING_WITH_SWINGS) {
					Point* point = new Point(x, y, 39, 30, 100);
					Grid* grid = Grid::GetInstance();
					grid->DeterminedGridToObtainObject(point);
					state = ENEMY_STATE_WALKING;
					mario->SetPoint(mario->GetPoint() + 100);
					vy = 0;
					mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
				}
			}
			else if (state == ENEMY_STATE_DIE) {
				mario->vy = -MARIO_JUMP_COLLISION_Y_WITH_ENEMY;
				if (mario->x > x)
					vx = -KOOPA_SPEED_TORTOISESHELL;
				else if (mario->x <= x)
					vx = KOOPA_SPEED_TORTOISESHELL;
				state = ENEMY_STATE_SPIN_DIE_KICK;
			}
		}
		if (collisionEvent->ny > 0) {
			if (!mario->GetUntouchable()) {
				if (mario->GetIsHolding()) {
					vector<LPGAMEOBJECT> enemies = scene->enemies;
					for (auto enemy : enemies) {
						if (Koopa* koopa = dynamic_cast<Koopa*> (enemy)) {
							if (koopa->GetIsHold()) {
								koopa->SetIsUpsideDown(false);
								koopa->noCollisionConsideration = false;
								koopa->y -= 40;
								koopa->SetState(ENEMY_STATE_WALKING);
								koopa->SetIsHold(false);
								koopa->vx = KOOPA_WALKING_SPEED_X * Mario::GetInstance()->nx;
								koopa->nx = Mario::GetInstance()->nx;
								Mario::GetInstance()->SetIsHolding(false);
							}
						}
					}
				}
				if (mario->GetLevel() >= MARIO_LEVEL_BIG) {
					mario->StartUntouchable();
					mario->y -= 5;
					mario->ChangeState(PlayerLevelDownTransformState::GetInstance());
					scene->StopGame(2000);
					mario->vx = 0;
					mario->vy = 0;
				}
				else if (Mario::GetInstance()->GetLevel() == MARIO_LEVEL_SMALL) {
					mario->y -= 5;
					Mario::GetInstance()->ChangeState(PlayerDieState::GetInstance());
					PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
					Mario::GetInstance()->noCollisionConsideration = true;
					scene->StopGame(5000);
				}
			}
			else {
				mario->y -= 0.4f;
			}
		}
	}
}

void Koopa::SetStartPosition()
{
	this->x = this->startPositionX;
	this->y = this->startPositionY;
	this->nx = this->startNx;
	this->vx = -KOOPA_WALKING_SPEED_X;
	/*if (Mario::GetInstance()->x > this->startPositionX)
		vx = KOOPA_WALKING_SPEED_X;
	else
		vx = -KOOPA_WALKING_SPEED_X;*/
	this->isUpsideDown = false;
	this->isDiedByFireball = false;
	if (startTypeMove == 1) {
		SetState(ENEMY_STATE_WALKING);
	}
	else if (startTypeMove == 2) {

		SetState(ENEMY_STATE_WALKING_WITH_SWINGS);
	}
}

void Koopa::CheckOverlapWithEnemy(vector<LPGAMEOBJECT> enemies)
{
	for (LPGAMEOBJECT x : enemies) {
		if (x->stillAlive && !x->noCollisionConsideration) {
			float leftBB, rightBB, topBB, bottomBB = .0f;
			float leftBBEnemy, rightBBEnemy, topBBEnemy, bottomBBEnemy = .0f;
			GetBoundingBox(leftBB, topBB, rightBB, bottomBB);
			x->GetBoundingBox(leftBBEnemy, topBBEnemy, rightBBEnemy, bottomBBEnemy);
			float widthBB = rightBB - leftBB;
			float heightBB = bottomBB - topBB;
			float widthEnemy = rightBBEnemy - leftBBEnemy;
			float heightEnemy = bottomBBEnemy - topBBEnemy;
			if ((leftBB + widthBB >= leftBBEnemy) && (leftBBEnemy + widthEnemy >= leftBB) && (topBB + heightBB >= topBBEnemy) && (topBBEnemy + heightEnemy >= topBB)) {
				if (LPENEMY enemy = dynamic_cast<LPENEMY> (x)) {
					if (enemy != NULL) {
						if (Koopa * koopa = dynamic_cast<Koopa*> (enemy)) {
							koopa->SetState(ENEMY_STATE_DIE);
							koopa->SetIsDiedByFireball();
						}
						if (enemy->x > Mario::GetInstance()->x) {
							enemy->vx = ENEMY_DIE_SPEED_X;
							vx = ENEMY_DIE_SPEED_X;
						}
						else {
							enemy->vx = -ENEMY_DIE_SPEED_X;
							vx = -ENEMY_DIE_SPEED_X;
						}
						enemy->vy = -ENEMY_DIE_SPEED_Y;
						enemy->SetIsUpsideDown(true);
						enemy->noCollisionConsideration = true;
						noCollisionConsideration = true;
						vy = -ENEMY_DIE_SPEED_Y*1.2f;
						isHold = false;
						isUpsideDown = true;
						isDiedByFireball = true;
						Mario::GetInstance()->ChangeState(PlayerStandingState::GetInstance());

					}
				}
			}
		}
	}
}
