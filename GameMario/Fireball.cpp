#include "Fireball.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "DynamicObjectDefine.h"
#include "Textures.h"
#include "game.h"
#include "MarioDefine.h"
#include "CollisionMapObject.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Camera.h"
#include "GraphicsDefine.h"
#include "EnemyDefine.h"
#include <algorithm>
#include "PlayerThrowingFireballState.h"
#include "PlayerLevelDownTransformState.h"

Fireball::Fireball()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	animation = animationDatabase->Get(FIREBALL_ANI_ROLLING);
}

Fireball::Fireball(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->gameObjectID = idGenerate++;
	this->vx = Mario::GetInstance()->nx * FIREBALL_ROLLING_SPEED_X;
}

void Fireball::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	if (isDie)
		animation = animationDatabase->Get(FIREBALL_ANI_DIE_EFFECT);
	else
		animation = animationDatabase->Get(FIREBALL_ANI_ROLLING);
}

void Fireball::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	if (nx < 1)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		if(!isDie || type == 2)
			animation->Render(x, y, alpha, scale);
		else {
			scale = D3DXVECTOR2(1, 1);
			animation->Render(x - 20, y, alpha, scale, -6);
			bool isLastFrame = animation->GetIsLastFrame();
			if (isLastFrame) {
				animation->ResetAnimation();
				isDie = true;
				stillAlive = false;
				PlayerThrowingFireballState::decreaseQuantityOneValue();
			}
		}
	}

//	RenderBoundingBox();
}

void Fireball::Update(DWORD dt)
{


	if (isDie)
		return;
	if (type == 1) {
		vy += FIREBALL_GRAVITY * dt * 2;
	}
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coObjects = scene->objects;
	vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	
	if (type == 1 && IsOverlapWithEnemy(coEnemies))
		return;

	coEvents.clear();
	CalcPotentialCollisions(&coObjects, coEvents);
	CalcPotentialCollisions(&coEnemies, coEvents);
	CalcPotentialCollisions(&coCollisionMapObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		Camera* camera = Camera::GetInstance();
		float cam_x, cam_y;
		camera->GetCamPos(cam_x, cam_y);
		if (x < cam_x || x > cam_x + SCREEN_WIDTH || y < cam_y || y > cam_y + SCREEN_HEIGHT) {
			PlayerThrowingFireballState::decreaseQuantityOneValue();
			stillAlive = false;
		}
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
		if (type == 1) {
			for (UINT i = 0; i < coEventsSize; i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (CollisionMapObject * collMapObj = dynamic_cast<CollisionMapObject*> (e->obj)) {
					CollisionWithOneCollisionMapObject(e, collMapObj);
				}
				else if (LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
					isDie = true;
					if (Koopa* koopa = dynamic_cast<Koopa*> (enemy)) {
						enemy->SetState(ENEMY_STATE_DIE);
						koopa->SetIsDiedByFireball();
					}
					if (enemy->x > Mario::GetInstance()->x) {
						enemy->vx = ENEMY_DIE_SPEED_X;
					}
					else {
						enemy->vx = -ENEMY_DIE_SPEED_X;
					}
					enemy->vy = -ENEMY_DIE_SPEED_Y;
					enemy->SetIsUpsideDown(true);
				}
				else {
					if (e->nx != 0) {
						vx = 0;
						isDie = true;
					}
					if (e->ny != 0 && e->nx == 0) {
						vy = -FIREBALL_ROLLING_SPEED_Y;
					}
					else if (e->ny != 0 && e->nx != 0) {
						vy = 0;
					}
				}

			}
		}
		else if (type == 2) {
			x += dx;
			y += dy;
			Camera* camera = Camera::GetInstance();
			float cam_x, cam_y;
			camera->GetCamPos(cam_x, cam_y);
			if (x < cam_x || x > cam_x + SCREEN_WIDTH || y < cam_y || y > cam_y + SCREEN_HEIGHT)
				stillAlive = false;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Fireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + FIREBALL_BBOX_WIDTH;
	b = t + FIREBALL_BBOX_HEIGHT;
}

bool Fireball::IsOverlapBoundingBoxWithMario()
{
	Mario* mario = Mario::GetInstance();
	float leftBBMario, rightBBMario, topBBMario, bottomBBMario = .0f;
	float leftBBFireball, rightBBFireball, topBBFireball, bottomBBFireball = .0f;
	mario->GetBoundingBox(leftBBMario, topBBMario, rightBBMario, bottomBBMario);
	GetBoundingBox(leftBBFireball, topBBFireball, rightBBFireball, bottomBBFireball);
	float widthEnemy = rightBBMario - leftBBMario;
	float heightEnemy = bottomBBMario - topBBMario;
	float widthFireball = rightBBFireball - leftBBFireball;
	float heightFireball = bottomBBFireball - topBBFireball;
	if ((leftBBMario + widthEnemy >= leftBBFireball) && (leftBBFireball + widthFireball >= leftBBMario) && (topBBMario + heightEnemy >= topBBFireball) && (topBBFireball + heightFireball >= topBBMario)) {
		return true;
	}
	return false;
}

bool Fireball::IsOverlapWithEnemy(vector<LPGAMEOBJECT> enemies)
{
	for (LPGAMEOBJECT x : enemies) {
		if (x->stillAlive) {
			float leftBBEnemy, rightBBEnemy, topBBEnemy, bottomBBEnemy = .0f;
			float leftBBFireball, rightBBFireball, topBBFireball, bottomBBFireball = .0f;
			x->GetBoundingBox(leftBBEnemy, topBBEnemy, rightBBEnemy, bottomBBEnemy);
			GetBoundingBox(leftBBFireball, topBBFireball, rightBBFireball, bottomBBFireball);
			float widthEnemy = rightBBEnemy - leftBBEnemy;
			float heightEnemy = bottomBBEnemy - topBBEnemy;
			float widthFireball = rightBBFireball - leftBBFireball;
			float heightFireball = bottomBBFireball - topBBFireball;

			if ((leftBBEnemy + widthEnemy >= leftBBFireball) && (leftBBFireball + widthFireball >= leftBBEnemy) && (topBBEnemy + heightEnemy >= topBBFireball) && (topBBFireball + heightFireball >= topBBEnemy)) {
				LPENEMY enemy = dynamic_cast<LPENEMY> (x);
				if (enemy != NULL) {
					isDie = true;
					if (dynamic_cast<Koopa*> (enemy))
						enemy->SetState(ENEMY_STATE_DIE);
					if (enemy->x > Mario::GetInstance()->x) {
						enemy->vx = ENEMY_DIE_SPEED_X;
					}
					else {
						enemy->vx = -ENEMY_DIE_SPEED_X;
					}
					enemy->vy = -ENEMY_DIE_SPEED_Y;
					enemy->SetIsUpsideDown(true);
					return true;
				}
			}
		}
	}
	return false;
}

void Fireball::CollisionWithOneCollisionMapObject(LPCOLLISIONEVENT collisionEvent, CollisionMapObject* collisionMapObject)
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
		else {
			isDie = true;
			vx = 0;
		}
	}
	if (collisionEvent->ny != 0) {
		if (collisionMapObjectDirectionY == 0)
			y += dy;
		else if (collisionEvent->ny < 0 && collisionMapObjectDirectionY == 1)
			y += dy;
		else if (collisionEvent->ny > 0 && collisionMapObjectDirectionY == -1)
			y += dy;
		else
			vy = -FIREBALL_ROLLING_SPEED_Y;
	}
}

