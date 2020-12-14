#include "WorldMap.h"

void WorldMap::_ParseSection_OBJECTS(string line)
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

		if (line == "[OBJECTS WORLD MAP]") {
			section = WORLDMAP_SECTION_OBJECT; continue;
		}

		switch (section)
		{
		case WORLDMAP_SECTION_OBJECT: ReadLineObject(line); break;
		}
	}

	f.close();
}

void WorldMap::ReadLineObject(string line)
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
	GateInWorldMap* gate = NULL;
	LPANIMATION_SET ani_set = NULL;
	ani_set = animation_sets->Get(ani_set_id);
	switch (object_type)
	{
		case OBJECT_TYPE_MARIO:
		{
			if (player != NULL)
			{
				player->nx = -1;
				player->ChangeState(PlayerInWorldMapState::GetInstance());
				player->SetPosition(115, 130);
				return;
			}
			Mario* mario = Mario::GetInstance();
			mario->SetLevel(MARIO_LEVEL_RACCOON);
			mario->ChangeState(PlayerInWorldMapState::GetInstance());
			mario->noCollisionConsideration = false;
			ani_set = AnimationManager::GetInstance()->Get(ani_set_id);

			mario->SetPosition(x, y);

			mario->SetAnimationSet(ani_set);
			mario->nx = -1;
			player = mario;

			DebugOut(L"[INFO] Player object created!\n");
			break;
		}
		case 11: {
			obj = new WorldMapObject(x, y, w, h, WORLDMAP_OBJECT_TYPE_CACTUS);
			ani_set = AnimationManager::GetInstance()->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
			objects.push_back(obj);
			break;
		}
		case 12: {
			obj = new WorldMapObject(x, y, w, h, WORLDMAP_OBJECT_TYPE_HELP_TAGS);
			ani_set = AnimationManager::GetInstance()->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
			objects.push_back(obj);
			break;
		}
		case 13: {
			obj = new WorldMapObject(x, y, w, h, WORLDMAP_OBJECT_TYPE_GUARD);
			ani_set = AnimationManager::GetInstance()->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
			objects.push_back(obj);
			break;
		}
		case 14: {
			int isInvisibleGate = atoi(tokens[6].c_str());
			int type = atoi(tokens[7].c_str());
			int directionX = atoi(tokens[8].c_str());
			int directionY = atoi(tokens[9].c_str());
			if (isInvisibleGate == 1)
				gate = new GateInWorldMap(x, y, w, h, type, false, directionX, directionY);
			else
				gate = new GateInWorldMap(x, y, w, h, type, true, directionX, directionY);
			ani_set = AnimationManager::GetInstance()->Get(ani_set_id);
			gate->SetAnimationSet(ani_set);
			gatesInWorldMap.push_back(gate);
			break;
		}
	default:
		break;
	}
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
	player->UpdateInWorldMap(dt);
	BoardGame* board = BoardGame::GetInstance();
	board->UpdateBoardGame();
}

void WorldMap::Render()
{
	MapManager* mapManager = MapManager::GetInstance();
	mapManager->RenderMap(mapID);
	BoardGame* board = BoardGame::GetInstance();
	board->RenderBoardGame();

	for (auto object : objects)
		object->Render();
	for (auto gate : gatesInWorldMap)
		gate->Render();

	player->Render();
}

void WorldMap::Unload()
{
	for (auto object : objects)
		delete object;
	for (auto gate : gatesInWorldMap)
		delete gate;
	objects.clear();
	gatesInWorldMap.clear();

	MapManager* mapManager = MapManager::GetInstance();
	if (mapManager->GetMap(this->mapID) != NULL) {
		mapManager->ClearMapById(this->mapID);
		delete mapManager->GetMap(this->mapID);
	}
	if (Textures::GetInstance()->GetTexture(ID_TEX_BOARDGAME) != NULL)
		Textures::GetInstance()->GetTexture(ID_TEX_BOARDGAME)->Release();

	DebugOut(L"[INFO] Scen unloaded! %s\n", sceneFilePath);
}
