#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprite.h"
#include "Animation.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define ID_TEX_BOARDGAME -200

class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class GameObject
{
protected:
	static int idGenerate;
public:
	int gameObjectID = 0;
	float x = .0f;
	float y = .0f;
	float w = .0f;
	float h = .0f;

	float dx = .0f;	// dx = vx*dt
	float dy = .0f;	// dy = vy*dt

	float vx = .0f;
	float vy = .0f;

	int nx = 0;

	DWORD dt = 0;

	bool stillAlive = true;
	bool inGrid = false;

	LPANIMATION_SET animation_set = NULL;

	float startPositionX = .0f;
	float startPositionY = .0f;
	int startNx = 0;
	int state = 0;
	bool isAlreadyAppeared = false;
	bool noCollisionConsideration = false;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	void SetIsAlreadyAppeared(bool isAlreadyAppeared) { this->isAlreadyAppeared = isAlreadyAppeared; };
	bool GetIsAlreadyAppeared() { return this->isAlreadyAppeared; };

	virtual void RenderBoundingBox();
	int GetState() { return this->state; };
	void SetState(int state) { this->state = state; };

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	virtual void SetStartPosition();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	GameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, int scaleTime);
	virtual void Render() = 0;
	void SetInGrid(bool inGrid) { this->inGrid = inGrid; };
	bool GetInGrid() { return this->inGrid; };

	~GameObject();
};

