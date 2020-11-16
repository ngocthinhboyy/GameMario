#pragma once
#include "GameObject.h"

#define CELL_WIDTH (SCREEN_WIDTH/4.0f)
#define CELL_HEIGHT (SCREEN_HEIGHT/4.0f)

#define MAX_ROW (int)((1968/CELL_HEIGHT) + 1)
#define MAX_COLUMN (int)((8848/CELL_WIDTH) + 1)
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
	void UpdateGrid(LPGAMEOBJECT object);
	static Grid* GetInstance();


};
