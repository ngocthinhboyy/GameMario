#include "Grid.h"
#include "Camera.h"
#include "Enemy.h"
#include "PlayScene.h"
#include "Debug.h"
#include <iostream>
#include <fstream>
#include "AnimationManager.h"
#include "PlayerStandingState.h"
#include "QuestionBrick.h"
#include "Flower.h"

Grid* Grid::__instance = NULL;
void Grid::LoadObjectInSceneAddToGrid(string line)
{


	if (line[0] == '#') return;
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return;
	//int id = atoi(tokens[0].c_str());
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
	case OBJECT_TYPE_GOOMBA: {
		int typeGoomba = atoi(tokens[6].c_str());
		obj = new Goomba(x, y, w, h, typeGoomba);
		//obj->gameObjectID = id;
		obj->SetPosition(x, y);

		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DeterminedGridToObtainObject(obj);
		break;
	}
	case OBJECT_TYPE_QUESTION_BRICK: {
		int type = atoi(tokens[6].c_str());
		obj = new QuestionBrick(x, y, w, h, type);
		//obj->gameObjectID = id;
		obj->SetPosition(x, y);
		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DeterminedGridToObtainObject(obj);
		break;
	}
	case OBJECT_TYPE_KOOPA: {
		int typeKoopa = atoi(tokens[6].c_str());
		int typeMove = atoi(tokens[7].c_str());
		obj = new Koopa(x, y, w, h, typeKoopa,typeMove);
		//obj->gameObjectID = id;
		obj->SetPosition(x, y);

		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DeterminedGridToObtainObject(obj);
		break;
	}
	case OBJECT_TYPE_FLOWER: {
		int type = atoi(tokens[6].c_str());
		obj = new Flower(x, y, w, h, type);
		//obj->gameObjectID = id;
		obj->SetPosition(x, y);
		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DeterminedGridToObtainObject(obj);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

}
void Grid::GetListObjectInCamera()
{
	for (auto obj : enemies) {
		obj->SetInGrid(false);
	}
	for (auto obj : objects) {
		obj->SetInGrid(false);
	}

	enemies.clear();
	objects.clear();

	float cam_x, cam_y;
	Camera* camera = Camera::GetInstance();
	camera->GetCamPos(cam_x, cam_y);
	int top = (int)((cam_y) / CELL_HEIGHT);
	int bottom = (int)((cam_y + SCREEN_HEIGHT) / CELL_HEIGHT);

	int left = (int)((cam_x) / CELL_WIDTH);
	int right = (int)((cam_x + SCREEN_WIDTH) / CELL_WIDTH);
	/*DebugOut(L"TOPPPP %d\n", top);
	DebugOut(L"BOTTOMMM %d\n", bottom);
	DebugOut(L"LEFFFTTT %d\n", left -1);
	DebugOut(L"RIGHTTTT %d\n", right );*/
	for (int i = top; i <= bottom; i++)
		for (int j = left - 1; j <= right; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				if (cells[i][j].at(k)->stillAlive) {
					if (LPENEMY enemy = dynamic_cast<LPENEMY> (cells[i][j].at(k))) {
						if (!enemy->GetInGrid()) {
							enemy->SetInGrid(true);
							enemies.push_back(enemy);
							//DebugOut(L"IIII %d\n", i);
							//DebugOut(L"JJJJJ %d\n", j);
						}
					}
					else if(!cells[i][j].at(k)->GetInGrid()){
						cells[i][j].at(k)->SetInGrid(true);
						objects.push_back(cells[i][j].at(k));
					}
				}
			}
		}
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	scene->enemies = enemies;
	scene->objects = objects;
	//DebugOut(L"SIZEEEEEEEE %d\n", objects.size());
	//DebugOut(L"OBJECT %d\n", objects.size());
	
}

void Grid::DeterminedGridToObtainObject(LPGAMEOBJECT object)
{
	int top = (int)(object->y / CELL_HEIGHT);
	int bottom = (int)((object->y + object->h) / CELL_HEIGHT);
	int left = (int)(object->x / CELL_WIDTH);
	int right = (int)((object->x + object->w) / CELL_WIDTH);
	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			cells[i][j].push_back(object);
		}
}
void Grid::UpdateGrid(LPGAMEOBJECT object)
{
	float cam_x, cam_y;
	Camera* camera = Camera::GetInstance();
	camera->GetCamPos(cam_x, cam_y);
	int topFocusCam = (int)((cam_y) / CELL_HEIGHT);
	int bottomFocusCam = (int)((cam_y + SCREEN_HEIGHT) / CELL_HEIGHT);

	int leftFocusCam = (int)((cam_x) / CELL_WIDTH);
	int rightFocusCam = (int)((cam_x + SCREEN_WIDTH) / CELL_WIDTH);

	for (int i = topFocusCam; i <= bottomFocusCam; i++)
		for (int j = leftFocusCam - 1; j <= rightFocusCam; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				cells[i][j].at(k)->SetInGrid(false);
				if (cells[i][j].at(k)->gameObjectID == object->gameObjectID) {
					cells[i][j].erase(cells[i][j].begin() + k);
				}
			}
		}
	int top = (int)(object->y / CELL_HEIGHT);
	int bottom = (int)((object->y + object->h) / CELL_HEIGHT);
	int left = (int)(object->x / CELL_WIDTH);
	int right = (int)((object->x + object->w) / CELL_WIDTH);

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			cells[i][j].push_back(object);
		}

	/*for (int i = topFocusCam; i <= bottomFocusCam; i++) {
		for (int j = 0; j < cells[i][leftFocusCam - 2].size(); j++)
			cells[i][leftFocusCam - 2].at(j)->SetStartPosition();
		for (int j = 0; j < cells[i][rightFocusCam + 1].size(); j++)
			cells[i][rightFocusCam + 1].at(j)->SetStartPosition();
	}*/
}
Grid* Grid::GetInstance()
{
	if (__instance == NULL) __instance = new Grid();
	return __instance;
}
