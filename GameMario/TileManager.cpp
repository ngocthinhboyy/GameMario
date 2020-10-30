#include "TileManager.h"
#include "Tile.h"
#include "Textures.h"
#include "debug.h"
#include "game.h"
#include <iostream>
#include <fstream>

TileManager::TileManager() {
	this->tile_height = TILE_HEIGHT;
	this->tile_width = TILE_WIDTH;
};

void TileManager::AddTile(int left, int top, int height, int width) {
	LPTILE tile = new Tile(left, top, height, width, this->tileset);
	tiles.push_back(tile);
}

void TileManager::CutTileset(LPCWSTR fileTexturePath, LPCWSTR fileTileSetPosition, D3DCOLOR transColor) {
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(fileTexturePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", fileTexturePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = Game::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		fileTexturePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transColor,
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}
	this->tileset = texture;

	DebugOut(L"[INFO] Start loading  resources from : %s \n", fileTileSetPosition);

	ifstream f;
	int left, top;
	f.open(fileTileSetPosition);
	while (f >> left >> top)
	{
		AddTile(left, top, this->tile_height, this->tile_width);
	}
}

TileManager::~TileManager() {};