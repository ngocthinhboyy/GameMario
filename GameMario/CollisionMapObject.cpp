#include "CollisionMapObject.h"
#include "Textures.h"
#include "game.h"
#include "debug.h"

CollisionMapObject::CollisionMapObject()
{
}

CollisionMapObject::CollisionMapObject(float w, float h, int colDirectX, int colDirectY)
{
	this->w = w;
	this->h = h;
	this->collisionDirectionX = colDirectX;
	this->collisionDirectionY = colDirectY;
}
void CollisionMapObject::RenderBoundingBox()
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
void CollisionMapObject::Render()
{
	//RenderBoundingBox();
}

void CollisionMapObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
