#include "Gate.h"
#include "Textures.h"
#include "Game.h"

Gate::Gate(float x, float y, float w, float h, int type, float cam_x, float cam_y, float newPositionMario, int wayDirectY)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->type = type;
	this->cam_x = cam_x;
	this->cam_y = cam_y;
	this->newPostitionMario = newPositionMario;
	this->wayDirectionY = wayDirectionY;
}

void Gate::Render()
{
	RenderBoundingBox();
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
