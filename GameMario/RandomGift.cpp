#include "RandomGift.h"
#include "ItemDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"
#include "Mario.h"
#include "PlayerGoingAutoState.h"
#include "Camera.h"
#include "EndTitle.h"
#include "BoardGame.h"

RandomGift::RandomGift(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->startPositionX = x;
	this->startPositionY = y;
	this->state = STATE_ITEM_CURRENT_FIRE_FLOWER;
	this->timeStartRandom = GetTickCount64();
	this->gameObjectID = idGenerate++;
}

void RandomGift::SetAnimation()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	switch (this->state) {
		case STATE_ITEM_CURRENT_FIRE_FLOWER: {
			this->animation = animationDatabase->Get(ITEM_CURRENT_FIRE_FLOWER_ANI);
			break;
		}
		case STATE_ITEM_CURRENT_MUSHROOM: {
			this->animation = animationDatabase->Get(ITEM_CURRENT_MUSHROOM_ANI);
			break;
		}
		case STATE_ITEM_CURRENT_WHITE_STAR: {
			this->animation = animationDatabase->Get(ITEM_CURRENT_WHITE_STAR_ANI);
			break;
		}
		case STATE_ALREADY_BONUS_GIFT:
			if(itemCurrent == 0)
				this->animation = animationDatabase->Get(ITEM_CURRENT_FIRE_FLOWER_ALREADY_BONUS_ANI);
			else if(itemCurrent == 1)
				this->animation = animationDatabase->Get(ITEM_CURRENT_MUSHROOM_ALREADY_BONUS_ANI);
			else if(itemCurrent == 2)
				this->animation = animationDatabase->Get(ITEM_CURRENT_WHITE_STAR_ALREADY_BONUS_ANI);
	}
}

void RandomGift::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(3,3);
	//this->animation->Render(7971, 915, alpha, scale);
	SetAnimation();
	if (animation != NULL) {
		AnimationDatabase::GetInstance()->Get(3601)->Render(startPositionX - 15, startPositionY - 15, alpha, scale);
		animation->Render(x, y, alpha, scale);
	}
	//RenderBoundingBox();
}

void RandomGift::Update(DWORD dt, int scaleTime)
{
	DWORD now = GetTickCount64();
	if (state != STATE_ALREADY_BONUS_GIFT) {
		if (now - timeStartRandom >= 100) {
			if (this->state == STATE_ITEM_CURRENT_FIRE_FLOWER) {
				this->state = STATE_ITEM_CURRENT_MUSHROOM;
				itemCurrent = 1;
				timeStartRandom = now;
			}
			else if (this->state == STATE_ITEM_CURRENT_MUSHROOM) {
				this->state = STATE_ITEM_CURRENT_WHITE_STAR;
				itemCurrent = 2;
				timeStartRandom = now;
			}
			else if (this->state == STATE_ITEM_CURRENT_WHITE_STAR) {
				this->state = STATE_ITEM_CURRENT_FIRE_FLOWER;
				itemCurrent = 0;
				timeStartRandom = now;
			}
		}
	}
	else {
		vy = -0.2f;
		GameObject::Update(dt,scaleTime);
		x += dx;
		y += dy;
		float cx, cy;
		Camera::GetInstance()->GetCamPos(cx, cy);

		if (y < cy) {
			if (itemCurrent == 0)
			{
				EndTitle::GetInstance()->SetCard(3);
				BoardGame::GetInstance()->SetIdCardForCardInBoard(3);
			}
			else if (itemCurrent == 1) {
				EndTitle::GetInstance()->SetCard(2);
				BoardGame::GetInstance()->SetIdCardForCardInBoard(2);
			}
			else if (itemCurrent == 2) {
				EndTitle::GetInstance()->SetCard(4);
				BoardGame::GetInstance()->SetIdCardForCardInBoard(4);
			}
			PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
			scene->SetIsEndScene(true);
			this->stillAlive = false;
		}
	}
}

void RandomGift::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + w;
	b = t + h;
}

void RandomGift::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	if (collisionEvent->nx != 0)
		Mario::GetInstance()->vx = 0;
	if (collisionEvent->ny != 0) {
		Mario::GetInstance()->vy = 0;
		if (collisionEvent > 0) {
			state = STATE_ALREADY_BONUS_GIFT;
			noCollisionConsideration = true;
			Mario::GetInstance()->ChangeState(PlayerGoingAutoState::GetInstance());

		}
	}
}
