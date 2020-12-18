#include "Gate.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"
#include "debug.h"
#include "PlayerFallingSlowlyState.h"
#include "Camera.h"
#include "PlayerStandingState.h"
#include "AnimationDatabase.h"
#include "PlayerFallingState.h"

Gate::Gate(float x, float y, float w, float h, int type, float cam_x, float cam_y, float newPositionXMario, float newPositionYMario, int wayDirectionY)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->cam_x = cam_x;
	this->cam_y = cam_y;
	this->newPostitionXMario = newPositionXMario;
	this->newPostitionYMario = newPositionYMario;
	this->wayDirectionY = wayDirectionY;
	this->gameObjectID = idGenerate++;
}

void Gate::Render()
{
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	D3DXVECTOR2 scale = D3DXVECTOR2(3, 3);
	LPANIMATION animation = animationDatabase->Get(2401);
	if (type == 2) {
		animation->Render(x, y, 255, scale);
	}
	else if (type == 1) {
		animation = animationDatabase->Get(2601);
		animation->Render(x, y, 255, scale);
	}
	
}

void Gate::Update(DWORD dt, int scaleTime)
{
	Mario* mario = Mario::GetInstance();
	if (isOverlapWithMario()) {
		isUsingThisGate = true;
	}
	if (isUsingThisGate) {
		if (isOverlapWithMario()) {
			if (wayDirectionY < 0) {
				if (mario->y > y) {
					Camera::GetInstance()->SetCamPos(cam_x, cam_y);
					Camera::GetInstance()->SetLockCamY(false);
					mario->x = newPostitionXMario;
					mario->y = newPostitionYMario;

					isUsingThisGate = false;
				}
			}
			else if (wayDirectionY > 0) {
				if (mario->y < y) {
					Camera::GetInstance()->SetCamPos(cam_x, cam_y);
					Camera::GetInstance()->SetLockCamY(false);
					mario->x = newPostitionXMario;
					mario->y = newPostitionYMario;
					isUsingThisGate = false;
				}
			}
		}
		else {
			mario->SetCanGoDownIntoGate(false);
			mario->SetIsMovingIntoGate(false);
			mario->SetCanGoUpIntoGate(false);
			if(mario->GetLevel() == MARIO_LEVEL_RACCOON)
				mario->ChangeState(PlayerFallingSlowlyState::GetInstance());
			else
				mario->ChangeState(PlayerFallingState::GetInstance());
			isUsingThisGate = false;
		}
	}

}

void Gate::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

void Gate::RenderBoundingBox()
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

	Game::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 50);
}

bool Gate::isOverlapWithMario()
{
	Mario* mario = Mario::GetInstance();
	float leftBBGate, rightBBGate, topBBGate, bottomBBGate = .0f;
	float leftBBMario, rightBBMario, topBBMario, bottomBBMario = .0f;
	GetBoundingBox(leftBBGate, topBBGate, rightBBGate, bottomBBGate);
	mario->GetBoundingBox(leftBBMario, topBBMario, rightBBMario, bottomBBMario);
	float widthGate = rightBBGate - leftBBGate;
	float heightGate = bottomBBGate - topBBGate;
	float widthMario = rightBBMario - leftBBMario;
	float heightMario = bottomBBMario - topBBMario;

	if ((leftBBMario + widthMario >= leftBBGate) && (leftBBGate + widthGate >= leftBBMario) && (topBBMario + heightMario >= topBBGate) && (topBBGate + heightGate >= topBBMario)) {
		return true;
	}
	return false;
}

void Gate::CollisionWithPlayer(LPCOLLISIONEVENT collisionEvent)
{
	Mario* mario = Mario::GetInstance();
	LPCOLLISIONEVENT e = collisionEvent;
	if (mario->GetIsMovingIntoGate()) {
		isUsingThisGate = true;
		mario->y += mario->dy;
	}
	if (e->ny != 0) {
		if(!mario->GetIsRunning())
			mario->vx = 0;
		mario->vy = 0;
		Mario::GetInstance()->SetIsOnGround(true);
		if (e->ny < 0 && wayDirectionY < 0) {
			mario->SetCanGoDownIntoGate(true);
		}
		else if (e->ny > 0 && wayDirectionY > 0) {
			mario->SetCanGoUpIntoGate(true);
		}
		else {
			mario->SetCanGoDownIntoGate(false);
			mario->SetCanGoUpIntoGate(false);
		}
	}
}
