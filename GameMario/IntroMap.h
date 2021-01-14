#pragma once
#include "Scene.h"
#include "GameObject.h"
#include <iostream>
#include <fstream>
#include "MarioInIntro.h"
class IntroMap : public Scene
{
protected:
	int mapID = 0;
	int timeOfPreviousSecond = 0;

	void _ParseSection_OBJECTS(string line);
	void ReadLineObject(string line);
	void _ParseSection_MAP(string line);

public:
	bool panelDisappeared = false;
	vector<LPGAMEOBJECT> objects;
	MarioInIntro* marioGreen = NULL;
	MarioInIntro* marioRed = NULL;
	IntroMap(int id, LPCWSTR filePath);


	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();
};

