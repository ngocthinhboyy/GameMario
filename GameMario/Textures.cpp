
#include "Textures.h"
#include "debug.h"
#include  "Game.h"

Textures* Textures::__instance = NULL;

Textures::Textures() {

}
Textures* Textures::GetInstance() {
	if (__instance == NULL) __instance = new Textures();
	return __instance;
}

Textures::~Textures()
{
}

void Textures::AddTexture(unsigned int id, LPCWSTR filePath, D3DCOLOR transparentColor) {
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = Game::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	textures[id] = texture;

	DebugOut(L"[INFO] Texture loaded Ok: id=%d, %s\n", id, filePath);
}

LPDIRECT3DTEXTURE9 Textures::GetTexture(unsigned int id) {
	return textures[id];
}

void Textures::Clear()
{
	for (auto texture : textures) {
		LPDIRECT3DTEXTURE9 tex = texture.second;
		if (tex != NULL) tex->Release();
	}

	textures.clear();
}