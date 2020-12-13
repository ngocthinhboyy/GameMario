#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "debug.h"
#include "Textures.h"
#include "SpriteManager.h"
#include "Gate.h"
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
#include "BoardGame.h"
#include "EndTitle.h"

using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new PlaySceneKeyHandler(this);
}

void PlayScene::StopGame(DWORD time)
{
	this->timeStartStoppingGame = GetTickCount64();
	this->timeStop = time;
	this->isStopGame = true;
	this->timeScale = 0;
}

void PlayScene::RestartGame()
{
	if (GetTickCount64() - timeStartStoppingGame >= timeStop) {
		this->timeStartStoppingGame = 0;
		this->timeStop = 0;
		this->isStopGame = false;
		this->timeScale = 1;
	}
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
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		mario->ChangeState(PlayerStandingState::GetInstance());
		mario->noCollisionConsideration = false;
		ani_set = AnimationManager::GetInstance()->Get(ani_set_id);

		mario->SetPosition(x, y);

		mario->SetAnimationSet(ani_set);
		player = mario;
		
		DebugOut(L"[INFO] Player object created!\n");
		break;
	}
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

void PlayScene::_ParseSection_REMAINING_TIME(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;
	this->remainingTime = atoi(tokens[0].c_str());

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
		if (line == "[REMAINING TIME]") {
			section = SCENE_SECTION_REMAINING_TIME; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_REMAINING_TIME: _ParseSection_REMAINING_TIME(line); break;
		}
	}

	f.close();


	Textures::GetInstance()->AddTexture(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	Textures::GetInstance()->AddTexture(ID_TEX_BOARDGAME, L"textures\\boardgame_panel.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void PlayScene::Update(DWORD dt)
{

	if (isStopGame)
		RestartGame();
	else {
		if (!isEndScene) {
			DWORD now = GetTickCount64();
			if (now - timeOfPreviousSecond >= 1000) {
				timeOfPreviousSecond = now;
				remainingTime--;
			}
		}
		else {
			DWORD now = GetTickCount64();
			if (now - timeOfPreviousSecond >= 10) {
				timeOfPreviousSecond = now;
				remainingTime--;
			}
		}
		if (remainingTime <= 0) {
			remainingTime = 0;
			Game::GetInstance()->SwitchScene(1);
			return;
		}
	}
	Grid* grid = Grid::GetInstance();
	grid->GetListObjectInCamera();


	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update(dt, this->timeScale);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, this->timeScale);
	}
	if(player != NULL)
		player->Update(dt, this->timeScale);

	for (size_t i = 0; i < enemies.size(); i++)
	{
		Grid::GetInstance()->SetNewGrid(enemies[i]);
		if (!enemies[i]->GetIsAlreadyAppeared())
			enemies.erase(enemies.begin() + i);
	}

	Grid::GetInstance()->SetStartPosition();
	if (player == NULL) return;

	BoardGame* board = BoardGame::GetInstance();
	board->UpdateBoardGame();


	float cx, cy;
	Camera::GetInstance()->UpdateCamPos();
	Camera::GetInstance()->GetCamPos(cx, cy);
	Camera::GetInstance()->SetCamPos(cx, cy);
}

void PlayScene::Render()
{
	MapManager* mapManager = MapManager::GetInstance();
	mapManager->RenderMap(mapID);


	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Render();
	}
	for (size_t i = 0; i < collisionMapObjects.size(); i++)
	{
		collisionMapObjects[i]->Render();
	}

	player->Render();
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Render();
	}
	BoardGame* board = BoardGame::GetInstance();
	board->RenderBoardGame();
	if (isEndScene) {
		EndTitle* endTitle = EndTitle::GetInstance();
		endTitle->RenderEndTitle();
	}
}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	
	for (auto object : objects)
		delete object;
	for (auto enemy : enemies)
		delete enemy;
	for (auto object : collisionMapObjects)
		delete object;

	objects.clear();
	enemies.clear();
	collisionMapObjects.clear();
	
	player = NULL;

	if(Grid::GetInstance() != NULL)
		Grid::GetInstance()->ClearGrid();

	MapManager* mapManager = MapManager::GetInstance();
	if (mapManager->GetMap(this->mapID) != NULL) {
		mapManager->ClearMapById(this->mapID);
		delete mapManager->GetMap(this->mapID);
	}
	if(Textures::GetInstance()->GetTexture(ID_TEX_BBOX) != NULL)
		Textures::GetInstance()->GetTexture(ID_TEX_BBOX)->Release();
	if(Textures::GetInstance()->GetTexture(ID_TEX_BOARDGAME) != NULL)
		Textures::GetInstance()->GetTexture(ID_TEX_BOARDGAME)->Release();

	DebugOut(L"[INFO] Scen unloaded! %s\n", sceneFilePath);
}
