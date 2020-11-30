#include "Tail.h"
#include "Textures.h"
#include "game.h"
#include "Mario.h"
#include "PlayScene.h"
#include "EnemyDefine.h"
#include "debug.h"
#include "AnimationDatabase.h"
#include "QuestionBrick.h"
#include "StaticObjectDefine.h"
#include "Leaf.h"
#include "ItemDefine.h"
#include "Coin.h"
#include "Grid.h"

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
	if (hasEffect) {
		AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
		D3DXVECTOR2 scale = D3DXVECTOR2(3, 3);
		animation = animationDatabase->Get(2201);
		animation->Render(x + 10, y, 255, scale);
	}
}

void Tail::Update(DWORD dt)
{
	GameObject::Update(dt);

	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());

	vector<LPGAMEOBJECT> coObjects = scene->enemies;
	for (auto x : scene->objects) {
		if (dynamic_cast<QuestionBrick*> (x))
			coObjects.push_back(x);
	}

	CheckOverlapBoundingBox(coObjects);
}

void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

void Tail::CheckOverlapBoundingBox(vector<LPGAMEOBJECT> objects)
{
	Mario* mario = Mario::GetInstance();
	for (LPGAMEOBJECT x : objects) {
		if (!x->stillAlive) return;
		float leftBB, rightBB, topBB, bottomBB = .0f;
		float leftBBTail, rightBBTail, topBBTail, bottomBBTail = .0f;
		x->GetBoundingBox(leftBB, topBB, rightBB, bottomBB);
		GetBoundingBox(leftBBTail, topBBTail, rightBBTail, bottomBBTail);
		float widthBB = rightBB - leftBB;
		float heightBB = bottomBB - topBB;
		float widthTail = rightBBTail - leftBBTail;
		float heightTail = bottomBBTail - topBBTail;

		if ((leftBB + widthBB >= leftBBTail) && (leftBBTail + widthTail >= leftBB) && (topBB + heightBB >= topBBTail) && (topBBTail + heightTail >= topBB)) {
			if (LPENEMY enemy = dynamic_cast<LPENEMY> (x)) {
				if (enemy != NULL) {
					hasEffect = true;
					if (Koopa* koopa = dynamic_cast<Koopa*> (enemy)) {
						koopa->SetTimeDie();
						koopa->SetState(ENEMY_STATE_DIE);
						koopa->vy = -ENEMY_DIE_SPEED_Y - 0.2f;
						koopa->SetIsUpsideDown(true);
						if (mario->x > koopa->x) {
							koopa->vx = -ENEMY_DIE_SPEED_X;
						}
						else {
							koopa->vx = ENEMY_DIE_SPEED_X;
						}
						//koopa->vx = mario->nx * ENEMY_DIE_SPEED_X;
					}
					else {
						enemy->vx = ENEMY_DIE_SPEED_X;
						if (enemy->x < mario->x) {
							enemy->vx = -ENEMY_DIE_SPEED_X;
						}
						enemy->vy = -ENEMY_DIE_SPEED_Y;
						enemy->SetIsUpsideDown(true);
					}
				}
			}
			else if (QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*> (x)) {
				if (!questionBrick->isEmptyBrick) {
					if (questionBrick->GetType() == QUESTION_BRICK_TYPE_HAS_ESPECIAL_ITEM) {
						if (Mario::GetInstance()->GetLevel() != MARIO_LEVEL_SMALL) {
							Leaf* leaf = new Leaf(questionBrick->x + QUESTION_BRICK_BBOX_WIDTH / 2 - LEAF_BBOX_WIDTH / 2, questionBrick->y - 3, LEAF_BBOX_WIDTH, LEAF_BBOX_HEIGHT);
							leaf->vy = -LEAF_SPEED_Y_APPEAR;
							leaf->vx = LEAF_SPEED;
							Grid::GetInstance()->DeterminedGridToObtainObject(leaf);
						}
					}
					else if (questionBrick->GetType() == QUESTION_BRICK_TYPE_HAS_COIN) {
						Coin* coin = new Coin(questionBrick->x + QUESTION_BRICK_BBOX_WIDTH / 2 - COIN_BBOX_WIDTH / 2, questionBrick->y - 3, COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT, 1);
						coin->vy = -COIN_SPEED_Y;
						Grid::GetInstance()->DeterminedGridToObtainObject(coin);
					}
				}
				questionBrick->isEmptyBrick = true;
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
