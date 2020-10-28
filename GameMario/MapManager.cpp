#include "MapManager.h"
MapManager* MapManager::__instance = NULL;
MapManager* MapManager::GetInstance() {
	if (__instance == NULL) __instance = new MapManager();
	return __instance;
}
void MapManager::AddMap(int id, LPCWSTR filePath) {
	LPMAP map = new Map(filePath);
	maps[id] = map;
}
LPMAP MapManager::GetMap(int id) {
	return maps[id];
}
void MapManager::RenderMap(int id) {
	maps[id]->DrawMap();
}
void MapManager::ReadMap(int id) {
	maps[id]->ReadMap();
}
MapManager::MapManager() {};
MapManager::~MapManager() {};