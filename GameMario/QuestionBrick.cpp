#include "QuestionBrick.h"
#include "AnimationDatabase.h"
#include "StaticObjectDefine.h"
#include "debug.h"

QuestionBrick::QuestionBrick()
{
}

QuestionBrick::QuestionBrick(float x, float y, float w, float h)
{
	this->x = x;
	this->start_Y_position = y;
	this->y = y;
	this->w = w;
	this->h = h;
	animation = AnimationDatabase::GetInstance()->Get(QUESTION_BRICK_ANI);
}



void QuestionBrick::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if(isEmptyBrick)
		animation = AnimationDatabase::GetInstance()->Get(QUESTION_BRICK_DIE_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void QuestionBrick::Update(DWORD dt)
{

	if (isEmptyBrick && !alreadyMoving) {
		if (vy == 0) {
			vy = -0.02 * dt;
		}
		else {
			if (y >= start_Y_position) {
				vy = 0;
				alreadyMoving = true;
				y = start_Y_position;
			}
			else
				vy += 0.005 *dt;
		}
		GameObject::Update(dt);
		y += dy;
	}
}

void QuestionBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x - QUESTION_BRICK_BBOX_WIDTH / 2;
	top = y - QUESTION_BRICK_BBOX_HEIGHT / 2;*/
	left = x;
	top = y;
	right = left + QUESTION_BRICK_BBOX_WIDTH;
	bottom = top + QUESTION_BRICK_BBOX_HEIGHT;
}
