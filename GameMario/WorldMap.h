#pragma once
#include <iostream>
#include <fstream>
#include "PlaySceneKeyHandler.h"
#include "debug.h"
#include "MapManager.h"
#include "Textures.h"
#include "LoadDefine.h"
#include "WorldMapDefine.h"
#include "Camera.h"
#include "BoardGame.h"
#include "AnimationManager.h"
#include "WorldMapObject.h"
#include "GateInWorldMap.h"
#include "PlayerInWorldMapState.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
class WorldMap : public Scene
{
protected:
	Mario* player;
	int mapID;
	int timeOfPreviousSecond = 0;

	void _ParseSection_OBJECTS(string line);
	void ReadLineObject(string line);
	void _ParseSection_MAP(string line);

public:
	vector<LPGAMEOBJECT> objects;
	vector<GateInWorldMap*> gatesInWorldMap;
	WorldMap(int id, LPCWSTR filePath);


	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();

	Mario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

