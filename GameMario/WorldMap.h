#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
class WorldMap : public Scene
{
protected:
	Mario* player;
	int mapID;
	int remainingTime = 0;
	int timeOfPreviousSecond = 0;

	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

public:
	vector<LPGAMEOBJECT> objects;
	WorldMap(int id, LPCWSTR filePath);


	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();

	Mario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

