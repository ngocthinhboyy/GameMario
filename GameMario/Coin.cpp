#include "Coin.h"
#include "AnimationDatabase.h"
#include "ItemDefine.h"
#include "debug.h"
#include "Mario.h"
#include "game.h"
#include "PlayScene.h"
#include "Point.h"
#include "Grid.h"

Coin::Coin(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->startPositionY = y;
	this->type = type;
	if (type == 3)
		timeStartTransform = GetTickCount64();
	this->gameObjectID = idGenerate++;
}

void Coin::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	if(type == 1 )
		this->animation = animationDatabase->Get(COIN_ANI);
	else if(type == 2)
		this->animation = animationDatabase->Get(STATIC_COIN_ANI);
	else if (type == 3)
		this->animation = animationDatabase->Get(COIN_INTO_ESPECIAL_BRICK_ANI);
}

void Coin::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	SetAnimation();
	scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	//RenderBoundingBox();
}

void Coin::Update(DWORD dt, int scaleTime)
{
	Mario* mario = Mario::GetInstance();
	if (type == 1) {
		if (!isBonused) {
			vy += ITEM_GRAVITY * dt;
			GameObject::Update(dt, scaleTime);
			if (y + dy + 10 >= startPositionY) {
				mario->SetPoint(mario->GetPoint() + MARIO_BONUS_POINT_COIN);
				this->isBonused = true;
				this->stillAlive = false;
			    Point* point = new Point(x, y, 39, 30, 100);
				Grid* grid = Grid::GetInstance(); 
				grid->DeterminedGridToObtainObject(point);
			}
			else {
				y += dy;
			}
		}
	}
	else if (type == 3) {
		if (GetTickCount64() - timeStartTransform >= 7500) {
			Grid::GetInstance()->DeterminedGridToObtainObject(new EspecialBrick(x, y, 48, 48, 1));
			stillAlive = false;
		}
	}
}

void Coin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void Coin::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	mario->SetPoint(mario->GetPoint() + MARIO_BONUS_POINT_STATIC_COIN);
	mario->SetCoin(mario->GetCoin() + 1);
	this->isBonused = true;
	this->stillAlive = false;
}

Coin::~Coin()
{
}
