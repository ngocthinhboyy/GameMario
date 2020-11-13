#include "Grid.h"
#include "Camera.h"
#include "Enemy.h"
#include "PlayScene.h"
#include "Debug.h"

Grid* Grid::__instance = NULL;
void Grid::GetListObjectInCamera()
{
	enemies.clear();
	objects.clear();

	float cam_x, cam_y;
	Camera* camera = Camera::GetInstance();
	camera->GetCamPos(cam_x, cam_y);
	int top = (int)((cam_y) / CELL_HEIGHT);
	int bottom = (int)((cam_y + SCREEN_HEIGHT) / CELL_HEIGHT);

	int left = (int)((cam_x) / CELL_WIDTH);
	int right = (int)((cam_x + SCREEN_WIDTH) / CELL_WIDTH);

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				if (cells[i][j].at(k)->stillAlive) {
					if (dynamic_cast<LPENEMY> (cells[i][j].at(k))) {
						DebugOut(L"DDDDDDDDDDDDDDDDDDd\n");
						DebugOut(L"i %d\n", i);
						DebugOut(L"j %d\n", j);
						AddEnemy(cells[i][j].at(k));
					}
					else
						AddObject(cells[i][j].at(k));
				}
			}
		}
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	//DebugOut(L"right %d\n", right);
	scene->enemies = enemies;
	DebugOut(L"sizee %d\n", scene->enemies.size());
	scene->objects = objects;
	
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

void Grid::AddEnemy(LPGAMEOBJECT enemy)
{
	enemies.push_back(enemy);
}

void Grid::AddObject(LPGAMEOBJECT object)
{
	objects.push_back(object);
}

Grid* Grid::GetInstance()
{
	if (__instance == NULL) __instance = new Grid();
	return __instance;
}
