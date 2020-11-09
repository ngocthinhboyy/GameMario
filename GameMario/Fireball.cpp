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

Fireball::Fireball()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	animation = animationDatabase->Get(FIREBALL_ANI_ROLLING);
}

Fireball::Fireball(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->vx = Mario::GetInstance()->nx * FIREBALL_ROLLING_SPEED_X;
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	animation = animationDatabase->Get(FIREBALL_ANI_ROLLING);
}

void Fireball::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	if (nx < 1)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}

	//RenderBoundingBox();
}

void Fireball::Update(DWORD dt)
{
	vy += FIREBALL_GRAVITY * dt * 2;
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coObjects = scene->objects;
	vector<LPGAMEOBJECT> coEnemies = scene->enemies;
	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

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
		if (x < cam_x || x > cam_x + SCREEN_WIDTH || y < cam_y || y > cam_y + SCREEN_HEIGHT)
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
			if (CollisionMapObject* collMapObj = dynamic_cast<CollisionMapObject*> (e->obj)) {
				CollisionWithOneCollisionMapObject(e, collMapObj);
			}
			else if (LPENEMY enemy = dynamic_cast<LPENEMY> (e->obj)) {
				stillAlive = false;
				if(dynamic_cast<Koopa*> (enemy))
					enemy->SetState(ENEMY_STATE_DIE);
				enemy->vx = ENEMY_DIE_SPEED_X;
				enemy->vy = -ENEMY_DIE_SPEED_Y;
				enemy->stillAlive = false;
			}
			else {
				if (e->nx != 0) {
					vx = 0;
					stillAlive = false;
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

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Fireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIREBALL_BBOX_WIDTH / 2;
	t = y - FIREBALL_BBOX_HEIGHT / 2;
	r = l + FIREBALL_BBOX_WIDTH;
	b = t + FIREBALL_BBOX_HEIGHT;
}

//void Fireball::CalcPotentialCollisionsWithEnemy(vector<LPENEMY>* coEnemies, vector<LPCOLLISIONEVENT>& coEvents)
//{
//	for (UINT i = 0; i < coEnemies->size(); i++)
//	{
//		LPCOLLISIONEVENT e = SweptAABBEx(coEnemies->at(i));
//
//		if (e->t > 0 && e->t <= 1.0f)
//			coEvents.push_back(e);
//		else
//			delete e;
//	}
//
//	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
//}

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
			stillAlive = false;
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

