#include "Goomba.h"
#include "EnemyDefine.h"
#include "MarioDefine.h"

Goomba::Goomba()
{
}

Goomba::Goomba(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	
}

void Goomba::Render()
{
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	vy += ENEMY_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
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
				CollisionWithCollisionMapObject(e, collMapObj);
			}
			else {
				if (e->nx != 0) vx = 0;
				if (e->ny != 0) vy = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Goomba::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GOOMBA_BBOX_WIDTH / 2;
	t = y - GOOMBA_BBOX_HEIGHT / 2;
	r = l + GOOMBA_BBOX_WIDTH;
	b = t + GOOMBA_BBOX_HEIGHT;
}

void Goomba::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, CollisionMapObject* collisionMapObject)
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
			vy = 0;
	}
}
