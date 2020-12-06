#include "Leaf.h"
#include "ItemDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "Mario.h"
#include "Camera.h"
#include "PlayScene.h"
#include "PlayerBonusTransformState.h"

Leaf::Leaf(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->gameObjectID = idGenerate++;
	this->startTime = GetTickCount64();
	this->startPositionX = x;
	this->startPositionY = y;
	this->nx = 1;
}

void Leaf::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	this->animation = animationDatabase->Get(LEAF_ANI);
}

void Leaf::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	if(nx > 0)
		scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	else if (nx < 0)
		scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	//RenderBoundingBox();
}

void Leaf::Update(DWORD dt, int scaleTime)
{
	vy += ITEM_GRAVITY * dt;
	if (vy >= 0) {
		vuamoirakhoi = false;
		if (y < startPositionY) {
			vy = ITEM_GRAVITY * dt * 2.5;
		}
		else {
			vy = 0;
			vy = ITEM_GRAVITY * dt * 2.5;
		}
		if (isCapVDau) {
			vx = 0.14f;
			isCapVDau = false;
		}
		if (x >= startPositionX + 50) {
			vx = -0.14f;
		}
		else if (x <= startPositionX - 50) {
			vx = 0.14f;
		}
	}
	GameObject::Update(dt, scaleTime);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();
	coObjects.push_back(Mario::GetInstance());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (x >= startPositionX + 48) {
			nx = -1;
		}
		else if (x <= startPositionX - 48) {
			nx = 1;
		}
		Camera* cam = Camera::GetInstance();
		float cam_x, cam_y;
		cam->GetCamPos(cam_x, cam_y);
		if (x < cam_x || x > cam_x + SCREEN_WIDTH || y < cam_y || y > cam_y + SCREEN_HEIGHT) {
			stillAlive = false;
		}
	}
	else
	{
		Mario* mario = Mario::GetInstance();
		if (mario->GetLevel() < MARIO_LEVEL_RACCOON)
		{
			mario->ChangeState(PlayerBonusTransformState::GetInstance());
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			scene->StopGame(700);
			mario->vx = 0;
			mario->vy = 0;
		}
		mario->y -= 5;
		mario->SetPoint(mario->GetPoint() + MARIO_BONUS_POINT_LEAF);
		this->stillAlive = false;

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Leaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void Leaf::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	//mario->SetLevel(mario->GetLevel() + 1);
	if (mario->GetLevel() < MARIO_LEVEL_RACCOON)
	{
		mario->ChangeState(PlayerBonusTransformState::GetInstance());
		PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
		scene->StopGame(700);
		mario->vx = 0;
		mario->vy = 0;
	}
	mario->y -= 5;
	mario->SetPoint(mario->GetPoint() + MARIO_BONUS_POINT_LEAF);
	this->stillAlive = false;

}

Leaf::~Leaf()
{
}
