#pragma once
#include "Scene.h"
#include "GameObject.h"
#include <iostream>
#include <fstream>
class IntroMap : public Scene
{
protected:
	int mapID;
	int timeOfPreviousSecond = 0;

	void _ParseSection_OBJECTS(string line);
	void ReadLineObject(string line);
	void _ParseSection_MAP(string line);

public:
	vector<LPGAMEOBJECT> objects;
	IntroMap(int id, LPCWSTR filePath);


	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();
};

