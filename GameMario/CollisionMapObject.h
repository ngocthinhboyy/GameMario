#pragma once
#include "GameObject.h"
class CollisionMapObject : public GameObject
{
	int collisionDirectionX;
	int collisionDirectionY;
public:
	CollisionMapObject();
	CollisionMapObject(float w, float h, int colDirectX, int colDirectY);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetCollisionDirectionX() { return this->collisionDirectionX; };
	int GetCollisionDirectionY() { return this->collisionDirectionY; };
	void RenderBoundingBox();
};

