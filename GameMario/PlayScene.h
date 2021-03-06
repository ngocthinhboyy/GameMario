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
	Mario* player = NULL;					// A play scene has to have player, right? 
	int mapID = 0;
	int timeOfPreviousSecond = 0;

	void _ParseSection_OBJECTS(string line);
	void _ParseSection_OBJECTS_NOT_IN_GRID(string line);
	void _ParseSection_OBJECTS_IN_GRID(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_REMAINING_TIME(string line);
	void _ParseSection_TYPE_CAMERA(string line);
	int timeScale = 1;
	DWORD timeStartStoppingGame = 0;
	DWORD timeStop = 0;
	bool isStopGame = false;
	bool isEndScene = false;
	bool isTimeUp = false;
	bool isAutoMovingCamera = false;

public:
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> collisionMapObjects;
	PlayScene(int id, LPCWSTR filePath);

	void StopGame(DWORD time);

	void RestartGame();
	int GetTimeOfPreviousSecond() { return this->timeOfPreviousSecond; };
	void SetTimeOfPreviousSecond(int timeOfPreviousSecond) { this->timeOfPreviousSecond = timeOfPreviousSecond; };

	void SetIsEndScene(bool isEndScene) { this->isEndScene = isEndScene; };
	bool GetIsEndScene() { return this->isEndScene; };
	bool GetIsAutoMovingCamera() { return this->isAutoMovingCamera; };
	void SetIsAutoMovingCamera(bool isAutoMovingCamera) { this->isAutoMovingCamera = isAutoMovingCamera; };
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Mario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

