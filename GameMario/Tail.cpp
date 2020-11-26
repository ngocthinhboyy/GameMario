#include "Tail.h"
#include "Textures.h"
#include "game.h"
#include "Mario.h"
#include "PlayScene.h"
#include "EnemyDefine.h"
#include "debug.h"

Tail::Tail()
{
}

Tail::Tail(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->gameObjectID = idGenerate++;
}

void Tail::Render()
{
	//RenderBoundingBox();
}

void Tail::Update(DWORD dt)
{
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coEnemies = scene->enemies;

	CheckOverlapBoundingBox(coEnemies);
}

void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

void Tail::CheckOverlapBoundingBox(vector<LPGAMEOBJECT> enemies)
{
	Mario* mario = Mario::GetInstance();
	for (LPGAMEOBJECT x : enemies) {
		if (!x->stillAlive) return;
		float leftBBEnemy, rightBBEnemy, topBBEnemy, bottomBBEnemy = .0f;
		float leftBBTail, rightBBTail, topBBTail, bottomBBTail = .0f;
		x->GetBoundingBox(leftBBEnemy, topBBEnemy, rightBBEnemy, bottomBBEnemy);
		GetBoundingBox(leftBBTail, topBBTail, rightBBTail, bottomBBTail);
		float widthEnemy = rightBBEnemy - leftBBEnemy;
		float heightEnemy = bottomBBEnemy - topBBEnemy;
		float widthTail = rightBBTail - leftBBTail;
		float heightTail = bottomBBTail - topBBTail;

		if ((leftBBEnemy + widthEnemy >= leftBBTail) && (leftBBTail + widthTail >= leftBBEnemy) && (topBBEnemy + heightEnemy >= topBBTail) && (topBBTail + heightTail >= topBBEnemy)) {
			LPENEMY enemy = dynamic_cast<LPENEMY> (x);
			if (enemy != NULL) {
				if (dynamic_cast<Koopa*> (enemy))
					enemy->SetState(ENEMY_STATE_DIE);
				enemy->vx = ENEMY_DIE_SPEED_X;
				if (enemy->x < mario->x) {
					enemy->vx = -ENEMY_DIE_SPEED_X;
				}
				enemy->vy = -ENEMY_DIE_SPEED_Y;
				enemy->SetIsUpsideDown(true);
			}
		}
	}
}

void Tail::RenderBoundingBox()
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

	Game::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 255);
}
