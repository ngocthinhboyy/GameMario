#pragma once
#include <d3dx9.h>
#include "GraphicsDefine.h"

class Sprite
{
	int id;

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;

public:
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture);

	void Draw(float x, float y, int alpha, D3DXVECTOR2 scale = D3DXVECTOR2(RATIO_X_SCALE,RATIO_Y_SCALE));

	~Sprite();
};

typedef Sprite* LPSPRITE;