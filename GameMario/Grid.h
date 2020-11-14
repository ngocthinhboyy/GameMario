#pragma once
#include "GameObject.h"

#define CELL_WIDTH (SCREEN_WIDTH/4.0f)
#define CELL_HEIGHT (SCREEN_HEIGHT/4.0f)

#define MAX_ROW 15
#define MAX_COLUMN 50
class Grid
{
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> objects;

	vector<LPGAMEOBJECT> cells[MAX_ROW][MAX_COLUMN];

	static Grid* __instance;
public:
	void LoadObjectInSceneAddToGrid(string line);
	void GetListObjectInCamera();
	void DeterminedGridToObtainObject(LPGAMEOBJECT object);
	static Grid* GetInstance();


};
