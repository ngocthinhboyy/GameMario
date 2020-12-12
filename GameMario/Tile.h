#pragma once
#include <d3dx9.h>
class Tile
{
	int left;
	int top;
	int height;
	int width;
	LPDIRECT3DTEXTURE9 texture;
public:
	Tile();
	Tile(int left, int top, int height, int width, LPDIRECT3DTEXTURE9 texture);
	void Draw(float x, float y);
	int GetHeight() { return this->height; };
	int GetWidth() { return this->width; };
	void ClearTextture();
	~Tile();
};
typedef Tile* LPTILE;

