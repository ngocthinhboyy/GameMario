#include "EspecialBrick.h"
#include "AnimationDatabase.h"
#include "StaticObjectDefine.h"
#include "Mario.h"
#include "FragmentOfEspecialBrick.h"
#include "Grid.h"
#include "ButtonP.h"

EspecialBrick::EspecialBrick(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->gameObjectID = idGenerate++;
	this->noCollisionConsideration = false;
}

void EspecialBrick::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	if (collisionEvent->ny != 0) {
		if (collisionEvent->ny > 0)
		{
			mario->SetIsOnGround(false);
			if (type == 1) {
				stillAlive = false;
				FragmentOfEspecialBrick* fragment1 = new FragmentOfEspecialBrick(x, y, 39, 39, -0.15, -1.1);
				FragmentOfEspecialBrick* fragment2 = new FragmentOfEspecialBrick(x, y, 39, 39, 0.15, -1.1);
				FragmentOfEspecialBrick* fragment3 = new FragmentOfEspecialBrick(x, y, 39, 39, -0.15, -0.8);
				FragmentOfEspecialBrick* fragment4 = new FragmentOfEspecialBrick(x, y, 39, 39, 0.15, -0.8);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment1);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment2);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment3);
				Grid::GetInstance()->DeterminedGridToObtainObject(fragment4);
			}
			else if (type == 2) {
				if (!isEmptyBrick) {
					isEmptyBrick = true;
					ButtonP* buttonP = new ButtonP(x, y, 48, 48);
					Grid::GetInstance()->DeterminedGridToObtainObject(buttonP);
				}
			}
		}
		else if (collisionEvent->ny < 0) {
			mario->SetIsOnGround(true);
		}
		mario->vy = 0;
	}
}

void EspecialBrick::Render()
{
	int alpha = 200;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	animation = AnimationDatabase::GetInstance()->Get(ESPECIAL_BRICK_ANI);
	if (type == 2 && isEmptyBrick) {
		animation = AnimationDatabase::GetInstance()->Get(QUESTION_BRICK_DIE_ANI);
	}
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void EspecialBrick::Update(DWORD dt, int scaleTime)
{
}

void EspecialBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
