#include "ButtonP.h"
#include "AnimationDatabase.h"
#include "Mario.h"
#include "StaticObjectDefine.h"
#include "PlayScene.h"
#include "Grid.h"
#include "Coin.h"
#include "debug.h"
#include "ItemDefine.h"

ButtonP::ButtonP(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->vx = 0;
	this->startPositionY = y;
	this->vy = -0.05f;
	this->gameObjectID = idGenerate++;
}

void ButtonP::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	if (collisionEvent->nx != 0)
		Mario::GetInstance()->vx = 0;
	if (collisionEvent->ny != 0) {
		Mario::GetInstance()->vx = 0;
		Mario::GetInstance()->vy = 0;
		if (collisionEvent->ny < 0) {
			Mario::GetInstance()->SetIsOnGround(true);
			if (!alreadyPress) {
				alreadyPress = true;
				h = 21;
				y += 27;
				PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
				for (auto x : scene->objects) {
					if (EspecialBrick* especialBrick = dynamic_cast<EspecialBrick*> (x)) {
						if (especialBrick->GetType() == ESPECIAL_BRICK_TYPE_HAS_EMPTY) {
							Grid::GetInstance()->DeterminedGridToObtainObject(new Coin(especialBrick->x, especialBrick->y, ESPECIAL_BRICK_WIDTH, ESPECIAL_BRICK_HEIGHT, COIN_TYPE_FROM_ESPECIAL_BRICK));
							x->stillAlive = false;
						}
					}
				}
			}
		}
	}
}

void ButtonP::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	if (alreadyPress)
		animation = AnimationDatabase::GetInstance()->Get(BUTTON_P_PRESSED_ANI);
	else
		animation = AnimationDatabase::GetInstance()->Get(BUTTON_P_ANI);
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
	RenderBoundingBox();
}

void ButtonP::Update(DWORD dt, int scaleTime)
{
	GameObject::Update(dt, scaleTime);
	x += dx;
	if (y + dy <= startPositionY - ESPECIAL_BRICK_HEIGHT) {
		vy = 0;
	}
	else
		y += dy;
}

void ButtonP::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
