#pragma once
#include <unordered_map>
#include <d3dx9.h>

using namespace std;

class Textures
{
	static Textures* __instance;
	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	Textures();
	void AddTexture(unsigned int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 GetTexture(unsigned int id);

	void Clear();
	static Textures* GetInstance();
	~Textures();
};
