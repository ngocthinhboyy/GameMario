#include "Map.h"
#include "debug.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include "game.h"
#include "Camera.h"

void Map::AddTileManager(LPTILEMANAGER tileManager) {
	this->tileManager = tileManager;
}
LPTILEMANAGER Map::GetTileManager() {
	return tileManager;
}
void Map::ReadMap() {

	DebugOut(L"[INFO] Start loading MAAPPPPPPPP resources from : %s \n", this->filePath);

	ifstream f;
	f.open(this->filePath);
	f >> this->row >> this->column >> this->height_board;
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->column; j++) {
			f >> this->mapArr[i][j];
		}
	}
}
void Map::DrawMap() {
	// get camera
	Camera* camera = Camera::GetInstance();
	float cam_x, cam_y;
	camera->GetCamPos( cam_x, cam_y);
	// get start end dong
	Game* game = Game::GetInstance();
	int screenHeight = game->GetScreenHeight();
	int screenWidth = game->GetScreenWidth();

	int frameWidth = tileManager->GetWidth();
	int frameHeight = tileManager->GetHeight();

	int startRow = cam_y / frameHeight;
	int endRow = (cam_y + screenHeight) / frameHeight + 1;

	int startColumn = cam_x / frameWidth;

	int endColumn = (cam_x + screenWidth) / frameWidth+ 1;

	vector<LPTILE> tiles = tileManager->GetTiles();
	for (int i = startRow; i < endRow; i++) {
		for (int j = startColumn; j < endColumn; j++) {
			int index = this->mapArr[i][j];
			if (index != 0) {
				tiles[index - 1]->Draw((j * frameWidth), (i) * frameHeight);
			}
		}
	}
}
void Map::ClearMap()
{
	memset(mapArr, 0, sizeof(mapArr[0][0]) * 500 * 500);
}
Map::Map(LPCWSTR filePath) {
	this->filePath = filePath;
}
Map::~Map() {};
