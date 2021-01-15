#include "Mushroom.h"
#include "AnimationDatabase.h"
#include "ItemDefine.h"
#include "PlayScene.h"
#include "Camera.h"
#include "QuestionBrick.h"
#include "Leaf.h"
#include "PlayerBonusTransformState.h"
#include "Point.h"
#include "Grid.h"
#include "Coin.h"
#include "PlayerStandingState.h"

Mushroom::Mushroom(float x, float y, float w, float h, int typeMushroom)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->gameObjectID = idGenerate++;
	this->startPositionY = y;
	this->typeMushroom = typeMushroom;
}

void Mushroom::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	if (typeMushroom == MUSHROOM_TYPE_RED) {
		this->animation = animationDatabase->Get(MUSHROOM_RED_ANI);
	}
	else if (typeMushroom == MUSHROOM_TYPE_GREEN) {
		this->animation = animationDatabase->Get(MUSHROOM_GREEN_ANI);
	}
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
}

void Mushroom::Update(DWORD dt, int scaleTime)
{
	if (y + MUSHROOM_BBOX_HEIGHT <= startPositionY && !alreadyAppear) {
		vy += ITEM_GRAVITY * dt;
		vx = MUSHROOM_SPEED * Mario::GetInstance()->nx;
		if (typeMushroom == MUSHROOM_TYPE_GREEN) {
			vx = MUSHROOM_SPEED;
		}
		alreadyAppear = true;
	}
	if (alreadyAppear) {
		vy += ITEM_GRAVITY * dt;
	}
	GameObject::Update(dt, scaleTime);
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coCollisionMapObjects = scene->collisionMapObjects;
	vector<LPGAMEOBJECT> coObjs = scene->objects;
	Mario* mario = Mario::GetInstance();
	coObjs.push_back(mario);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&coCollisionMapObjects, coEvents);
	CalcPotentialCollisions(&coObjs, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		Camera* cam = Camera::GetInstance();
		float cam_x, cam_y;
		cam->GetCamPos(cam_x, cam_y);
		if (x < cam_x || x > cam_x + SCREEN_WIDTH || y < cam_y || y > cam_y + SCREEN_HEIGHT) {
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
			else if (Coin * coin = dynamic_cast<Coin*> (e->obj)) {
				if (e->ny != 0) y += dy;
				if (e->nx != 0) x += dx;
			}
			else if (Mario * mario = dynamic_cast<Mario*> (e->obj)) {
				Point* point = NULL;
				mario->vx = 0;
				mario->vy = 0;
				mario->y -= 2;
				if (typeMushroom == MUSHROOM_TYPE_RED) {
					mario->ChangeState(PlayerBonusTransformState::GetInstance());
					point = new Point(x, y, 45, 24, 1000);
					mario->SetPoint(mario->GetPoint() + MARIO_BONUS_POINT_LEAF);
					PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
					scene->StopGame(1000);
				}
				else if (typeMushroom == MUSHROOM_TYPE_GREEN) {
					point = new Point(x, y, 45, 24, 1);
					mario->SetPoint(mario->GetPoint() + 100);
				}
				Grid* grid = Grid::GetInstance();
				if (point != NULL)
					grid->DeterminedGridToObtainObject(point);
				this->stillAlive = false;
			}
			else if (LPENEMY enemy = dynamic_cast<LPENEMY>(e->obj)) {
				if (e->ny != 0) y += dy;
				if (e->nx != 0) x += dx;
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
	Point* point = NULL;
	mario->vx = 0;
	mario->y -= 2;
	if (typeMushroom == MUSHROOM_TYPE_RED) {
		mario->vy = 0;
		mario->ChangeState(PlayerBonusTransformState::GetInstance());
		point = new Point(x, y, 45, 24, 1000);
		mario->SetPoint(mario->GetPoint() + MARIO_BONUS_POINT_LEAF);
		PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
		scene->StopGame(1000);
	}
	else if (typeMushroom == MUSHROOM_TYPE_GREEN) {
		point = new Point(x, y, 45, 24, 1);
		mario->SetPoint(mario->GetPoint() + 100);
		mario->SetHeart(mario->GetHeart() + 1);
	}
	Grid* grid = Grid::GetInstance();
	if (point != NULL)
		grid->DeterminedGridToObtainObject(point);
	this->stillAlive = false;
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
