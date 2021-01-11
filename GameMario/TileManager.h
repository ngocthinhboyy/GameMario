#pragma once
#include <vector>
#include "Tile.h"

#include "GraphicsDefine.h"
using namespace std;

class TileManager
{
	int tile_height = 0;
	int tile_width = 0;
	LPDIRECT3DTEXTURE9 tileset = NULL;
	vector<LPTILE> tiles;
public:

	TileManager();

	void AddTile(int left, int top, int height, int width);
	void ReadTileset(LPCWSTR fileTexturePath, LPCWSTR fileTileSetPosition, D3DCOLOR transColor);
	void ClearTileset();
	int GetWidth() { return tile_width; };
	int GetHeight() { return tile_height; };

	vector<LPTILE> GetTiles() { return this->tiles; }

	~TileManager();
};

typedef TileManager* LPTILEMANAGER;

