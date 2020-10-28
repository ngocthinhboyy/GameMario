#include "SpriteManager.h"
#include "debug.h"

SpriteManager* SpriteManager::__instance = NULL;

SpriteManager* SpriteManager::GetInstance() {
	if (__instance == NULL) __instance = new SpriteManager();
	return __instance;
}

void SpriteManager::AddSprite(int id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture)
{
	/*int right = left + width;
	int bottom = right + height;*/
	LPSPRITE s = new Sprite(id, left, top, width, height, texture);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, width, height);
}

LPSPRITE SpriteManager::GetSprite(int id) {
	return sprites[id];
}
void SpriteManager::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}