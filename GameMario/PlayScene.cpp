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

using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new PlaySceneKeyHandler(this);
}



void PlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->AddTexture(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int w = atoi(tokens[3].c_str());
	int h = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->GetTexture(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	SpriteManager::GetInstance()->AddSprite(ID, l, t, w+l, h+t, tex);
}

void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	AnimationDatabase::GetInstance()->Add(ani_id, ani);
}

void PlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new AnimationSet();

	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animationDatabase->Get(ani_id);
		s->push_back(ani);
	}

	AnimationManager::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 5) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	float w = atof(tokens[3].c_str());
	float h = atof(tokens[4].c_str());

	int ani_set_id = atoi(tokens[5].c_str());

	AnimationManager* animation_sets = AnimationManager::GetInstance();

	GameObject* obj = NULL;

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
		LPANIMATION_SET ani_set = AnimationManager::GetInstance()->Get(ani_set_id);
		player = mario;
		obj = mario;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	}
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: {
		obj = new Brick(w,h); 
		break;
	}
	case OBJECT_TYPE_KOOPAS: obj = new Koopas(); break;
	case OBJECT_TYPE_PORTAL:
		break;
	case OBJECT_TYPE_COLLISION_MAP: {
		int collisionDirectionX = atoi(tokens[6].c_str());
		int collisionDirectionY = atoi(tokens[7].c_str());
		obj = new CollisionMapObject(w,h, collisionDirectionX, collisionDirectionY);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
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
	tileManager->CutTileset(filePathMapTextureTileSet.c_str(), filePathMapTileSetPosition.c_str(), D3DCOLOR_XRGB(R, G, B));

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

		/*if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}*/
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		/*case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;*/
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
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
		//DebugOut(L"STILL ALIVEEEEE %d\n", objects[i]->stillAlive);
		if (!(objects[i]->stillAlive)) {
			objects.erase(objects.begin() + i);
		}
	}

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

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
