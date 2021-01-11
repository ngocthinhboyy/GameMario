#pragma once
#include "TileManager.h"
#include <unordered_map>
class Map
{
	int column = 0;
	int row = 0;
	int height_board = 0;
	int mapArr[500][500];
	LPTILEMANAGER tileManager = NULL;
	LPCWSTR filePath = NULL;
public:
	Map(LPCWSTR filePath);
	void AddTileManager(LPTILEMANAGER tileManager);
	LPTILEMANAGER GetTileManager();
	void ReadMap();
	void DrawMap();
	void ClearMap();
	int GetColumn() { return this->column; };
	~Map();
};

typedef Map* LPMAP;

