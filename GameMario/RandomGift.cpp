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
			if(itemCurrent == ITEM_FIRE_FLOWER)
				this->animation = animationDatabase->Get(ITEM_CURRENT_FIRE_FLOWER_ALREADY_BONUS_ANI);
			else if(itemCurrent == ITEM_MUSHROOM)
				this->animation = animationDatabase->Get(ITEM_CURRENT_MUSHROOM_ALREADY_BONUS_ANI);
			else if(itemCurrent == ITEM_WHITE_STAR)
				this->animation = animationDatabase->Get(ITEM_CURRENT_WHITE_STAR_ALREADY_BONUS_ANI);
	}
}

void RandomGift::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE,RATIO_Y_SCALE);
	SetAnimation();
	if (animation != NULL) {
		AnimationDatabase::GetInstance()->Get(RANDOM_GIFT_ANI)->Render(startPositionX - 15, startPositionY - 15, alpha, scale);
		animation->Render(x, y, alpha, scale);
	}
}

void RandomGift::Update(DWORD dt, int scaleTime)
{
	DWORD now = GetTickCount64();
	if (state != STATE_ALREADY_BONUS_GIFT) {
		if (now - timeStartRandom >= TIME_CHANGE_RANDOM_GIFT) {
			if (this->state == STATE_ITEM_CURRENT_FIRE_FLOWER) {
				this->state = STATE_ITEM_CURRENT_MUSHROOM;
				itemCurrent = ITEM_MUSHROOM;
				timeStartRandom = now;
			}
			else if (this->state == STATE_ITEM_CURRENT_MUSHROOM) {
				this->state = STATE_ITEM_CURRENT_WHITE_STAR;
				itemCurrent = ITEM_WHITE_STAR;
				timeStartRandom = now;
			}
			else if (this->state == STATE_ITEM_CURRENT_WHITE_STAR) {
				this->state = STATE_ITEM_CURRENT_FIRE_FLOWER;
				itemCurrent = ITEM_FIRE_FLOWER;
				timeStartRandom = now;
			}
		}
	}
	else {
		vy = -RANDOM_GIFT_SPEED_Y;
		GameObject::Update(dt,scaleTime);
		x += dx;
		y += dy;
		float cx, cy;
		Camera::GetInstance()->GetCamPos(cx, cy);

		if (y < cy) {
			if (itemCurrent == ITEM_FIRE_FLOWER)
			{
				EndTitle::GetInstance()->SetCard(ID_CARD_FIRE_FLOWER);
				BoardGame::GetInstance()->SetIdCardForCardInBoard(ID_CARD_FIRE_FLOWER);
			}
			else if (itemCurrent == ITEM_MUSHROOM) {
				EndTitle::GetInstance()->SetCard(ID_CARD_MUSHROOM);
				BoardGame::GetInstance()->SetIdCardForCardInBoard(ID_CARD_MUSHROOM);
			}
			else if (itemCurrent == ITEM_WHITE_STAR) {
				EndTitle::GetInstance()->SetCard(ID_CARD_WHITE_STAR);
				BoardGame::GetInstance()->SetIdCardForCardInBoard(ID_CARD_WHITE_STAR);
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
	if (collisionEvent->nx != 0) {
		Mario::GetInstance()->vx = 0;
		Mario::GetInstance()->vy = 0;
		state = STATE_ALREADY_BONUS_GIFT;
		noCollisionConsideration = true;
		Mario::GetInstance()->ChangeState(PlayerGoingAutoState::GetInstance());
	};
	if (collisionEvent->ny != 0) {
		Mario::GetInstance()->vy = 0;
		if (collisionEvent->ny > 0) {
			state = STATE_ALREADY_BONUS_GIFT;
			noCollisionConsideration = true;
			Mario::GetInstance()->ChangeState(PlayerGoingAutoState::GetInstance());

		}
	}
}
