#include "Leaf.h"
#include "ItemDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "Mario.h"
#include "Camera.h"
#include "PlayScene.h"

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
	RenderBoundingBox();
}

void Leaf::Update(DWORD dt)
{
	vx = 0;
	vy += ITEM_GRAVITY * dt;
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coObjs;
	coObjs.push_back(Mario::GetInstance());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&coObjs, coEvents);

	//if (coEvents.size() == 0)
	//{
		if (vy >= 0) {
			if (y < startPositionY) {
				vy = ITEM_GRAVITY * dt * 2;
			}
			else {
				vy = 0;
				vy = ITEM_GRAVITY * dt * 2;
			}
			float time = GetTickCount64() - startTime;
			if (cos(vx * time) > 0)
				vx = -LEAF_SPEED;
			else if (cos(vx * time) < 0) {
				vx = LEAF_SPEED;
			}
			x = startPositionX - LEAF_AMPLITUDE * cos(abs(vx) * time);
			//DebugOut(L"TIME %f\n",cos(60));
			if (x >= startPositionX + 58) {
				nx = -1;
			}
			else if (x <= startPositionX - 58) {
				nx = 1;
			}
		}
		y += dy;
		Camera* cam = Camera::GetInstance();
		float cam_x, cam_y;
		cam->GetCamPos(cam_x, cam_y);
		if (y > cam_y + SCREEN_HEIGHT) {
			this->stillAlive = false;
		}
	//}
	/*else
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
			if (e->nx != 0)
				DebugOut(L"SSSS \n");
			Mario* mario = Mario::GetInstance();
			mario->SetLevel(mario->GetLevel() + 1);
			this->stillAlive = false;
			mario->vx = 0;
			mario->vy = 0;
			mario->y -= 50;
		}
	}*/

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
	mario->SetLevel(mario->GetLevel() + 1);
	this->stillAlive = false;
	mario->vx = 0;
	mario->vy = 0;
	mario->y -= 50;
}

Leaf::~Leaf()
{
}
