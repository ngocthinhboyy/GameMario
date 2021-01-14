#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"
class MarioInIntro : public GameObject
{
	int type = 0;
	int state = 0;
	LPANIMATION animation = NULL;
	DWORD timeStartMoving = 0;
	bool isWalking = false;
	bool alreadyJumping = false;
public:
	MarioInIntro(float x, float y, float w, float h, int type);
	void Update(DWORD dt, int scaleTime);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetAnimation();
	void SetState(int state);
	void ActionInIntro();

	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	~MarioInIntro();
};

