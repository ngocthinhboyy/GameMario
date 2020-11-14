#pragma once
#include <vector>
#include "Tile.h"

#include "GraphicsDefine.h"
using namespace std;

class TileManager
{
	int tile_height;
	int tile_width;
	LPDIRECT3DTEXTURE9 tileset;
	vector<LPTILE> tiles;
public:

	TileManager();

	void AddTile(int left, int top, int height, int width);
	void ReadTileset(LPCWSTR fileTexturePath, LPCWSTR fileTileSetPosition, D3DCOLOR transColor);
	int GetWidth() { return tile_width; };
	int GetHeight() { return tile_height; };

	vector<LPTILE> GetTiles() { return this->tiles; }

	~TileManager();
};

typedef TileManager* LPTILEMANAGER;

