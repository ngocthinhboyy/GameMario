#include "WorldMap.h"
#include "PlaySceneKeyHandler.h"
#include "debug.h"
#include "MapManager.h"
#include "Textures.h"
#include <iostream>
#include <fstream>
#include "LoadDefine.h"
#include "Camera.h"
#include "BoardGame.h"

void WorldMap::_ParseSection_OBJECTS(string line)
{
}

void WorldMap::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	this->mapID = atoi(tokens[0].c_str());
	wstring filePathMapTxt = ToWSTR(tokens[1]);
	wstring filePathMapTextureTileSet = ToWSTR(tokens[2]);
	wstring filePathMapTileSetPosition = ToWSTR(tokens[3]);
	int R = atoi(tokens[4].c_str());
	int G = atoi(tokens[5].c_str());
	int B = atoi(tokens[6].c_str());

	MapManager* mapManager = MapManager::GetInstance();
	mapManager->AddMap(mapID, filePathMapTxt.c_str());

	LPTILEMANAGER tileManager = new TileManager();
	tileManager->ReadTileset(filePathMapTextureTileSet.c_str(), filePathMapTileSetPosition.c_str(), D3DCOLOR_XRGB(R, G, B));

	mapManager->GetMap(mapID)->AddTileManager(tileManager);
	mapManager->ReadMap(mapID);
	mapManager->RenderMap(mapID);
}

WorldMap::WorldMap(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new PlaySceneKeyHandler(this);
}

void WorldMap::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();


	Textures::GetInstance()->AddTexture(ID_TEX_BOARDGAME, L"textures\\boardgame_panel.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading world map resources %s\n", sceneFilePath);
}

void WorldMap::Update(DWORD dt)
{
	Camera::GetInstance()->SetCamPos(0, 0);
}

void WorldMap::Render()
{
	MapManager* mapManager = MapManager::GetInstance();
	mapManager->RenderMap(mapID);
	BoardGame* board = BoardGame::GetInstance();
	board->RenderBoardGame();
}

void WorldMap::Unload()
{
}
