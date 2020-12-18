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
#include "EspecialBrick.h"
#include "Flower.h"
#include "Coin.h"
#include "Gate.h"
#include "RandomGift.h"
#include "Mushroom.h"

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
	int top = atoi(tokens[5].c_str());
	int bottom = atoi(tokens[6].c_str());
	int left= atoi(tokens[7].c_str());
	int right = atoi(tokens[8].c_str());

	int ani_set_id = atoi(tokens[9].c_str());

	AnimationManager* animation_sets = AnimationManager::GetInstance();

	GameObject* obj = NULL;
	LPANIMATION_SET ani_set = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_GOOMBA: {
		int typeGoomba = atoi(tokens[10].c_str());
		obj = new Goomba(x, y, w, h, typeGoomba);
		obj->SetPosition(x, y);

		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
		break;
	}
	case OBJECT_TYPE_QUESTION_BRICK: {
		int type = atoi(tokens[10].c_str());
		obj = new QuestionBrick(x, y, w, h, type);
		obj->SetPosition(x, y);
		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
		break;
	}
	case OBJECT_TYPE_KOOPA: {
		int typeKoopa = atoi(tokens[10].c_str());
		int typeMove = atoi(tokens[11].c_str());
		obj = new Koopa(x, y, w, h, typeKoopa,typeMove);
		obj->SetPosition(x, y);

		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
		break;
	}
	case OBJECT_TYPE_FLOWER: {
		int type = atoi(tokens[10].c_str());
		obj = new Flower(x, y, w, h, type);
		obj->SetPosition(x, y);
		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
		break;
	}
	case OBJECT_TYPE_COIN: {
		int type = atoi(tokens[10].c_str());
		obj = new Coin(x, y, w, h, type);

		obj->SetPosition(x, y);
		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
		break;
	}
	case OBJECT_TYPE_GATE: {
		int type = atoi(tokens[10].c_str());
		float cam_x = atof(tokens[11].c_str());
		float cam_y = atof(tokens[12].c_str());
		float newPositionXMario = atof(tokens[13].c_str());
		float newPositionYMario = atof(tokens[14].c_str());
		int wayDirectionY = atoi(tokens[15].c_str());
		obj = new Gate(x, y, w, h, type, cam_x, cam_y, newPositionXMario, newPositionYMario, wayDirectionY);
		obj->SetPosition(x, y);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
		break;
	}
	case OBJECT_TYPE_ESPECIAL_BRICK: {
		int type = atoi(tokens[10].c_str());
		obj = new EspecialBrick(x, y, w, h, type);
		obj->SetPosition(x, y);
		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
		break;
	}
	case OBJECT_TYPE_RANDOM_GIFT: {
		obj = new RandomGift(x, y, w, h);
		obj->SetPosition(x, y);
		ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		AddObjectInFileToGrid(top, bottom, left, right, obj);
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
	int bottom = (int)((cam_y + 692) / CELL_HEIGHT);

	int left = (int)((cam_x) / CELL_WIDTH);
	int right = (int)((cam_x + 745) / CELL_WIDTH);
	/*DebugOut(L"TOPPP %d\n", top);
	DebugOut(L"BOTTOMMM %d\n", bottom);
	DebugOut(L"LEFFFTTT %d\n", left);
	DebugOut(L"RIGHTTTT %d\n", right);*/
	if (left < 3) {
		left = 3;
	}
	if (right >= 19) {
		right = 19;
	}
	if (bottom > 4)
		bottom = 4;
	vector<LPGAMEOBJECT> itemsInGame;
	LPGAMEOBJECT mushroom =  NULL;
	for (int i = top; i <= bottom + 1; i++)
		for (int j = left - 3 ; j <= right + 3; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				if ((cells[i][j].at(k)->x + cells[i][j].at(k)->w >= cam_x && cells[i][j].at(k)->x <= cam_x + 745) || cells[i][j].at(k)->GetIsAlreadyAppeared()) {
					if (cells[i][j].at(k)->stillAlive) {
						cells[i][j].at(k)->SetIsAlreadyAppeared(true);
						if (LPENEMY enemy = dynamic_cast<LPENEMY> (cells[i][j].at(k))) {
							if (!enemy->GetInGrid()) {
								enemy->SetInGrid(true);
								enemies.push_back(enemy);
							}
						}
						else if (LPITEM item = dynamic_cast<LPITEM> (cells[i][j].at(k))) {
							if (dynamic_cast<Mushroom*>(item)) {
								if (!item->GetInGrid()) {
									item->SetInGrid(true);
									mushroom = item;
								}
							}
							else {
								if (!item->GetInGrid()) {
									item->SetInGrid(true);
									itemsInGame.push_back(item);
								}
							}
						}
						else if (!cells[i][j].at(k)->GetInGrid()) {
							cells[i][j].at(k)->SetInGrid(true);
							objects.push_back(cells[i][j].at(k));
						}
					}
				}
			}
		}
	for (auto x : itemsInGame)
		objects.push_back(x);
	if (mushroom != NULL)
		objects.insert(objects.begin(), mushroom);
	itemsInGame.clear();
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	scene->enemies = enemies;
	scene->objects = objects;
//	DebugOut(L"SIZEEEEEEEE %d\n", objects.size());
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
void Grid::AddObjectInFileToGrid(int top, int bottom, int left, int right, LPGAMEOBJECT object)
{
	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			cells[i][j].push_back(object);
		}
}
void Grid::SetNewGrid(LPGAMEOBJECT object)
{
	float cam_x, cam_y;
	Camera* camera = Camera::GetInstance();
	camera->GetCamPos(cam_x, cam_y);

	int topFocusCam = (int)((cam_y) / CELL_HEIGHT);
	int bottomFocusCam = (int)((cam_y + 692) / CELL_HEIGHT);

	int leftFocusCam = (int)((cam_x) / CELL_WIDTH);
	int rightFocusCam = (int)((cam_x + 745) / CELL_WIDTH);
	if (leftFocusCam > 1)
		leftFocusCam = leftFocusCam - 2;
	else if (leftFocusCam > 0)
		leftFocusCam--;
	if (bottomFocusCam > 4)
		bottomFocusCam = 4;
	for (int i = topFocusCam; i <= bottomFocusCam + 1; i++)
		for (int j = leftFocusCam; j <= rightFocusCam + 1; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				if (cells[i][j].at(k)->gameObjectID == object->gameObjectID) {
					cells[i][j].erase(cells[i][j].begin() + k);
				}
			}
		}
	int top = (int)(object->y / CELL_HEIGHT);
	int bottom = (int)((object->y + object->h) / CELL_HEIGHT);
	int left = (int)(object->x / CELL_WIDTH);
	int right = (int)((object->x + object->w) / CELL_WIDTH);

	if (leftFocusCam > 1)
		leftFocusCam = leftFocusCam + 2;
	else if (leftFocusCam > 0)
		leftFocusCam++;

	if (left < leftFocusCam - 1 || right > rightFocusCam + 1 || bottom > bottomFocusCam + 1) {
		if (object->stillAlive) {
			object->SetStartPosition();
			object->SetIsAlreadyAppeared(false);
			objectsWaitingToSetStartPosition.push_back(object);
		}
	}
	else {
		for (int i = top; i <= bottom; i++)
			for (int j = left; j <= right; j++) {
				cells[i][j].push_back(object);
			}
	}

}
void Grid::SetStartPosition()
{
	float cam_x, cam_y;
	Camera* camera = Camera::GetInstance();
	camera->GetCamPos(cam_x, cam_y);


	for (int i = 0; i < objectsWaitingToSetStartPosition.size(); i++) {
		LPGAMEOBJECT object = objectsWaitingToSetStartPosition.at(i);
		if (object->x + object->w < cam_x || object->x > cam_x + 745) {
			DeterminedGridToObtainObject(object);
			objectsWaitingToSetStartPosition.erase(objectsWaitingToSetStartPosition.begin() + i);
		}
	}

}
void Grid::ClearGrid()
{
	enemies.clear();
	objects.clear();
	objectsWaitingToSetStartPosition.clear();
	for (int i = 0; i < MAX_ROW; i++)
		for (int j = 0; j < MAX_COLUMN; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				//cells[i][j].erase(cells[i][j].begin() + k);
				cells[i][j].clear();
			//delete cells[i][j].at(k);
		}
	}
}
Grid* Grid::GetInstance()
{
	if (__instance == NULL) __instance = new Grid();
	return __instance;
}
