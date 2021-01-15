#pragma once
#include "GameObject.h"
#include "CollisionMapObject.h"
class DynamicObjectInIntro : public GameObject
{
	int type = 0;
	LPANIMATION animation = NULL;

	int startTime = 0;
	bool isSupplyFirstSpeed = true;
public:

	DynamicObjectInIntro(float x, float y, float w, float h, int type);
	void Update(DWORD dt, int scaleTime);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetAnimation();

	void CollisionWithCollisionMapObject(LPCOLLISIONEVENT collisionEvent, LPCOLLISIONMAPOBJECT collisionMapObject);
	void CollisionWithMario(LPCOLLISIONEVENT collisionEvent, DynamicObjectInIntro* object);
	bool IsOverlapWithMario();
	~DynamicObjectInIntro();
};

