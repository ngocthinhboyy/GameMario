#pragma once
#include "Map.h"

class MapManager
{
	static MapManager* __instance;
	unordered_map <int, LPMAP> maps;
public:
	MapManager();
	void AddMap(int id, LPCWSTR filePath);
	LPMAP GetMap(int id);
	void ReadMap(int id);
	void RenderMap(int id);
	static MapManager* GetInstance();
	~MapManager();
};
