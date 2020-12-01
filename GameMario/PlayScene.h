#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Enemy.h"


class PlayScene : public Scene
{
protected:
	Mario* player;					// A play scene has to have player, right? 
	int mapID;
	int remainingTime = 0;
	int timeOfPreviousSecond = 0;

	void _ParseSection_OBJECTS(string line);
	void _ParseSection_OBJECTS_NOT_IN_GRID(string line);
	void _ParseSection_OBJECTS_IN_GRID(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_REMAINING_TIME(string line);
	int timeScale = 1;


public:
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> collisionMapObjects;
	PlayScene(int id, LPCWSTR filePath);

	int GetRemainingTime() { return this->remainingTime; };
	int GetTimeOfPreviousSecond() { return this->timeOfPreviousSecond; };
	void SetTimeOfPreviousSecond(int timeOfPreviousSecond) { this->timeOfPreviousSecond = timeOfPreviousSecond; };

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Mario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

