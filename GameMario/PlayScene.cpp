#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "debug.h"
#include "Textures.h"
#include "SpriteManager.h"
#include "Portal.h"
#include "PlaySceneKeyHandler.h"
#include "AnimationManager.h"
#include "AnimationDatabase.h"
#include "MapManager.h"
#include "TileManager.h"
#include "Camera.h"
#include "PlayerStandingState.h"
#include "CollisionMapObject.h"
#include "LoadDefine.h"
#include "QuestionBrick.h"
#include "Grid.h"

using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new PlaySceneKeyHandler(this);
}

void PlayScene::_ParseSection_OBJECTS(string line)
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

		if (line == "[OBJECTS NOT IN GRID]") {
			section = SCENE_SECTION_OBJECTS_NOT_IN_GRID; continue;
		}
		if (line == "[OBJECTS IN GRID]") {
			section = SCENE_SECTION_OBJECTS_IN_GRID; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		switch (section)
		{
			case SCENE_SECTION_OBJECTS_NOT_IN_GRID: _ParseSection_OBJECTS_NOT_IN_GRID(line); break;
			case SCENE_SECTION_OBJECTS_IN_GRID: _ParseSection_OBJECTS_IN_GRID(line); break;
		}
	}

	f.close();
	
}

void PlayScene::_ParseSection_OBJECTS_NOT_IN_GRID(string line)
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


	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			return;
		}
		Mario* mario = Mario::GetInstance();
		mario->SetLevel(MARIO_LEVEL_FIRE);
		mario->ChangeState(PlayerStandingState::GetInstance());
		ani_set = AnimationManager::GetInstance()->Get(ani_set_id);

		mario->SetPosition(x, y);

		mario->SetAnimationSet(ani_set);
		player = mario;
		
		DebugOut(L"[INFO] Player object created!\n");
		break;
	}
	case OBJECT_TYPE_PORTAL:
		break;
	case OBJECT_TYPE_COLLISION_MAP: {
		int collisionDirectionX = atoi(tokens[6].c_str());
		int collisionDirectionY = atoi(tokens[7].c_str());
		obj = new CollisionMapObject(w, h, collisionDirectionX, collisionDirectionY);
		obj->SetPosition(x, y);

		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		collisionMapObjects.push_back(obj);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
}

void PlayScene::_ParseSection_OBJECTS_IN_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 0) return;
	Grid* grid = Grid::GetInstance();
	grid->LoadObjectInSceneAddToGrid(line);
}

void PlayScene::_ParseSection_MAP(string line)
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

void PlayScene::Load()
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


	Textures::GetInstance()->AddTexture(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void PlayScene::Update(DWORD dt)
{
	Grid* grid = Grid::GetInstance();
	grid->GetListObjectInCamera();


	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update(dt);
		/*if (!(enemies[i]->stillAlive)) {
			enemies.erase(enemies.begin() + i);
		}*/
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt);
		//if (!(objects[i]->stillAlive)) {
			//objects.erase(objects.begin() + i);
		//}
	}
	player->Update(dt);
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	Game* game = Game::GetInstance();

	if (cx < (game->GetScreenWidth() / 2))
		cx = 0.0f;
	else {
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;
	}

	Camera::GetInstance()->SetCamPos(cx, CAM_Y_DRAW_MAP/*cy*/);
}

void PlayScene::Render()
{
	MapManager* mapManager = MapManager::GetInstance();
	mapManager->RenderMap(mapID);

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Render();
	}
	//DebugOut(L"ENEMI %d\n", enemies.size());
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Render();
	}
	player->Render();
}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scen unloaded! %s\n", sceneFilePath);
}
