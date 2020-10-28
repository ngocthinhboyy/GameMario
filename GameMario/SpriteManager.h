#pragma once

#include <unordered_map>
#include "Sprite.h"

using namespace std;


class SpriteManager
{
	static SpriteManager* __instance;

	unordered_map<int, LPSPRITE> sprites;
public:
	void AddSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture);
	LPSPRITE GetSprite(int id);
	void Clear();
	static SpriteManager* GetInstance();
};
