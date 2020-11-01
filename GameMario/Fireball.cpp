#include "Fireball.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "DynamicObjectDefine.h"
#include "Textures.h"
#include "game.h"
#include "MarioDefine.h"
#include "CollisionMapObject.h"
#include "Mario.h"

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
	this->vx = 0.4f;
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

	RenderBoundingBox();
}

void Fireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	vy += MARIO_GRAVITY * dt * 2;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (GetTickCount() - untouchable_start > FIREBALL_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

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
			if (e->nx != 0) vx = 0;
			if (e->ny != 0 && e->nx == 0) {
				vy = -0.6f;
			}
			else if (e->ny != 0 && e->nx != 0) {
				vy = 0;
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

void Fireball::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->GetTexture(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float draw_x = x - ((int)r - (int)l) / 2;
	float draw_y = y - ((int)b - (int)t) / 2;

	Game::GetInstance()->Draw(draw_x, draw_y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}
