#include "Tile.h"
#include "game.h"

Tile::Tile() {};
Tile::Tile(int left, int top, int height, int width, LPDIRECT3DTEXTURE9 texture) {
	this->left = left;
	this->top = top;
	this->height = height;
	this->width = width;
	this->texture = texture;
}
void Tile::Draw(float x, float y) {
	Game* game = Game::GetInstance();
	game->Draw(x, y, texture, left, top, left+this->width, top+this->height);
}

void Tile::ClearTextture()
{
	if (texture != NULL)
		texture->Release();
}

Tile::~Tile() {}