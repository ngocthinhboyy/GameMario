#pragma once
#include "TileManager.h"
#include <unordered_map>
class Map
{
	int column;
	int row;
	int height_board;
	int mapArr[500][500];
	LPTILEMANAGER tileManager;
	LPCWSTR filePath;
public:
	Map();
	Map(LPCWSTR filePath);
	void AddTileManager(LPTILEMANAGER tileManager);
	LPTILEMANAGER GetTileManager();
	void ReadMap();
	void DrawMap();
	void ClearMap();
	~Map();
};

typedef Map* LPMAP;

