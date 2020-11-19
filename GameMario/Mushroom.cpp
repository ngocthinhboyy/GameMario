#include "Mushroom.h"
#include "AnimationDatabase.h"
#include "ItemDefine.h"
#include "PlayScene.h"
#include "Camera.h"
#include "QuestionBrick.h"

Mushroom::Mushroom(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->gameObjectID = idGenerate++;
	this->startPositionY = y;
}

void Mushroom::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	this->animation = animationDatabase->Get(MUSHROOM_ANI);
}

void Mushroom::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void Mushroom::Update(DWORD dt)
{
	//vy += ITEM_GRAVITY * dt;
	if (y + MUSHROOM_BBOX_HEIGHT <= startPositionY && !alreadyAppear) {
		vy += ITEM_GRAVITY * dt;
		vx = MUSHROOM_SPEED * Mario::GetInstance()->nx;
		alreadyAppear = true;
	}
	if (alreadyAppear) {
		vy += ITEM_GRAVITY * dt;
	}
	GameObject::Update(dt);
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjs = scene->objects;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
	CalcPotentialCollisions(&coObjs, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		float cam_x, cam_y;
		Camera* camera = Camera::GetInstance();
		camera->GetCamPos(cam_x, cam_y);
		if (y > (cam_y + SCREEN_HEIGHT) || x > (cam_x + SCREEN_WIDTH) || x < cam_x)
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
			else if (QuestionBrick * brick = dynamic_cast<QuestionBrick*> (e->obj)) {
				if (e->ny != 0) vy = 0;
				if (e->nx != 0) vx = - vx;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void Mushroom::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	mario->SetLevel(mario->GetLevel() + 1);
	this->stillAlive = false;
	mario->vx = 0;
	mario->vy = 0;
	mario->y -= 50;
}

void Mushroom::CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject)
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
			vx = -vx;
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

Mushroom::~Mushroom()
{
}
