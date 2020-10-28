#pragma once
#include <string>
#include "LoadDefine.h"
#include <Windows.h>
using namespace std;
class ResourcesSharedLoad
{
public:
	ResourcesSharedLoad();
	void ParseTextureShared(string line);
	void ParseSpriteShared(string line);
	void ParseAnimationShared(string line);
	void ParseAnimationSetShared(string line);
	void LoadSharedResource(LPCWSTR filePath);
	~ResourcesSharedLoad();
};

