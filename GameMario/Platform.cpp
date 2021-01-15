#include "Platform.h"
#include "AnimationDatabase.h"
#include "IntroMapDefine.h"
#include "MarioInIntro.h"
#include "IntroMap.h"
#include "game.h"
#include "AnimationManager.h"
#include "debug.h"
#include "DynamicObjectInIntro.h"

Platform::Platform(float x, float y, float w, float h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	if (type == TYPE_STAGE)
		this->noCollisionConsideration = true;
	else if (type == TYPE_RED_CURTAIN)
		this->timeStart = GetTickCount64();
	else if (type == TYPE_PANEL)
		vy = 0;
}

void Platform::Update(DWORD dt, int scaleTime)
{
	if (type == TYPE_RED_CURTAIN) {
		if (GetTickCount64() - timeStart >= 2000 && !stopMoving) {
			vy = -0.3f;
			if (GetTickCount64() - timeStart >= 5000) {
				stopMoving = true;
				stillAlive = false;
				AnimationManager* animation_sets = AnimationManager::GetInstance();
				LPANIMATION_SET ani_set = NULL;
				ani_set = animation_sets->Get(21);
				MarioInIntro* marioGreen = new MarioInIntro(0,530,42,81,TYPE_GREEN_MARIO);
				MarioInIntro* marioRed = new MarioInIntro(700,530,42,81,TYPE_RED_MARIO);
				marioRed->SetAnimationSet(ani_set);
				marioGreen->SetAnimationSet(ani_set);
				IntroMap* scene = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());
				scene->marioGreen = marioGreen;
				scene->marioRed = marioRed;
				vy = 0;
			}
		}
		if (!stopMoving) {
			GameObject::Update(dt, scaleTime);
			y += dy;
			x += dx;
		}
	}
	else if (type == TYPE_PANEL) {
		if (y >= 103 && !stopMoving) {
			vy = 0;
			IntroMap* scene = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());
			for (auto object : scene->objects) {
				if (Platform* platform = dynamic_cast<Platform*> (object)) {
					if (platform->type == TYPE_STAGE) {
						platform->stillAlive = false;
					}
				}
			}
			scene->panelDisappeared = true;
			stopMoving = true;
			GameObject* object = new DynamicObjectInIntro(Game::GetInstance()->GetScreenWidth() / 2, 0, 48, 48, TYPE_ENEMY_KOOPA_TORTOISE);
			scene->objects.push_back(object);
			GameObject* leaf = new DynamicObjectInIntro(Game::GetInstance()->GetScreenWidth() / 2, 0, 48, 48, TYPE_LEAF);
			scene->objects.push_back(leaf);
		}
		if (!stopMoving) {
			GameObject::Update(dt, scaleTime);
			y += dy;
			x += dx;
		}
	}
}

void Platform::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (type == TYPE_STAGE)
		animation = AnimationDatabase::GetInstance()->Get(4702);
	else if (type == TYPE_RED_CURTAIN)
		animation = AnimationDatabase::GetInstance()->Get(4701);
	else if(type == TYPE_PANEL)
		animation = AnimationDatabase::GetInstance()->Get(4707);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void Platform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + w;
	bottom = y + h;
}

Platform::~Platform()
{
}
