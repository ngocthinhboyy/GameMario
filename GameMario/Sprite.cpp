#include "Sprite.h"
#include "Game.h"
#include "Camera.h"

Sprite::Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture) {

	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = texture;
}
void Sprite::Draw(float x, float y, int alpha, D3DXVECTOR2 scale,int offset)
{
	LPD3DXSPRITE spriteHandler = Game::GetInstance()->GetSpriteHandler();
	D3DXMATRIX newMatrix;
	Camera* camera = Camera::GetInstance();

	float cam_x, cam_y;
	camera->GetCamPos(cam_x, cam_y);
	x = x + (right * abs(scale.x) - left * abs(scale.x)) / 2;
	y = y + (bottom * abs(scale.y) -top * abs(scale.y)) / 2;
	D3DXVECTOR3 p(x - cam_x, y - cam_y, 0);

	RECT r;
	r.left = this->left;
	r.top = this->top;
	r.right = this->right;
	r.bottom = this->bottom;
	D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(p.x, p.y), 0, &scale, NULL, NULL, NULL);

	D3DXVECTOR3 center = D3DXVECTOR3((float)(right - left) / 2 + offset, (float)(bottom - top) / 2, 0);
	D3DXMATRIX oldMatrix;

	spriteHandler->GetTransform(&oldMatrix);
	spriteHandler->SetTransform(&newMatrix);

	spriteHandler->Draw(texture, &r, &center, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	
	spriteHandler->SetTransform(&oldMatrix);
}

Sprite::~Sprite() {}