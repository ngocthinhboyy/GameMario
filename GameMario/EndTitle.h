#pragma once
#include "game.h"
#include <fstream>
#include <iostream>
#include "Sprite.h"
struct ComponentInEndTitle {
	float x;
	float y;
	int idFont;
	int line;
};
class EndTitle
{
	static EndTitle* __instance;
	vector<ComponentInEndTitle*>  endTitleFontComponents;
	ComponentInEndTitle* endTitleCardComponent;
	vector<LPSPRITE> endTitleItem;
	DWORD renderFinishedOneLine = 0;
	int currentLine = 0;
public:

	void LoadEndTitleItem(LPCWSTR filePath);
	void ParseEndTitleItem(string line);

	void RenderEndTitle();

	void LoadEndTitle(LPCWSTR filePath);
	void ParsePositionInEndTitle(string line);

	void SetCard(int idCardItem) { endTitleCardComponent->idFont = idCardItem; };

	static EndTitle* GetInstance();
};

