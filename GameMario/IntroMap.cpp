#include "IntroMap.h"
#include "IntroMapKeyHandler.h"
#include "debug.h"
#include "LoadDefine.h"
#include "MapManager.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "ThreeObject.h"
#include "Arrow.h"

void IntroMap::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int objectSourceId = atoi(tokens[0].c_str());
	LPCWSTR filePathObject = ToLPCWSTR(tokens[1]);
	ifstream f;

	f.open(filePathObject);

	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;

		if (line == "[OBJECTS INTRO MAP]") {
			section = WORLDMAP_SECTION_OBJECT; continue;
		}

		switch (section)
		{
		case WORLDMAP_SECTION_OBJECT: ReadLineObject(line); break;
		}
	}

	f.close();
}

void IntroMap::ReadLineObject(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return;

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	float w = atof(tokens[3].c_str());
	float h = atof(tokens[4].c_str());

	int ani_set_id = atoi(tokens[5].c_str());

	AnimationManager* animation_sets = AnimationManager::GetInstance();

	GameObject* obj = NULL;
	LPANIMATION_SET ani_set = NULL;
	ani_set = animation_sets->Get(ani_set_id);
	switch (object_type)
	{
	case 15: {
		obj = new ThreeObject(x, y, w, h);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
		break;
	}
	case 16: {
		obj = new Arrow(x, y, w, h);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
		break;
	}
	default:
		break;
	}
}

void IntroMap::_ParseSection_MAP(string line)
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

IntroMap::IntroMap(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new IntroMapKeyHandler(this);
}

void IntroMap::Load()
{
	DebugOut(L"[INFO] Start loading intro map scene resources from : %s \n", sceneFilePath);

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

	DebugOut(L"[INFO] Done loading intro map resources %s\n", sceneFilePath);
}

void IntroMap::Update(DWORD dt)
{
}

void IntroMap::Render()
{
	Camera::GetInstance()->SetCamPos(0, 0);
	MapManager* mapManager = MapManager::GetInstance();
	mapManager->RenderMap(mapID);
	for (auto object : objects)
		object->Render();
}

void IntroMap::Unload()
{

	for (auto object : objects)
		delete object;
	objects.clear();
	MapManager* mapManager = MapManager::GetInstance();
	if (mapManager->GetMap(this->mapID) != NULL) {
		mapManager->ClearMapById(this->mapID);
		delete mapManager->GetMap(this->mapID);
	}
	DebugOut(L"[INFO] Scen unloaded! %s\n", sceneFilePath);

}
