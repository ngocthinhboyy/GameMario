#pragma once
#include "Sprite.h"
#include <fstream>
#include <iostream>
#include "game.h"
#include "debug.h"
#include "Textures.h"
struct Panel {
	float x;
	float y;
	float w;
	float h;
};
struct ComponentPositionInBoard {
	float x;
	float y;
	int idPosition;
	int idFontRender;
};
typedef ComponentPositionInBoard* LPCOMPONENTFONT;
class BoardGame
{
	static BoardGame* __instance;
	vector<LPSPRITE> fonts;
	vector<LPSPRITE> cards;
	vector<ComponentPositionInBoard*> componentPositionInBoards;
	vector<LPCOMPONENTFONT> componentTimeUp;
	vector <ComponentPositionInBoard*> componentCardInBoard;
	Panel panelBoardGame;
	DWORD timeRenderWhite = 0;
	bool renderWhiteP = true;
	bool firstTimeTurnOnMaxSpeed = false;

	int idCardRenderFirstTime = 0;
	bool firstTimeRenderCard = false;
	DWORD timeRenderCard = 0;
	bool renderCardInWhenFirstTime = true;
public:
	void LoadBoardGameItems(LPCWSTR filePath);
	void LoadPositionInBoardGame(LPCWSTR filePath);
	void ParseInBoardBoardGame(string line);
	int GetFontNumber(int number);
	void ParseFontBoardGame(string line);
	void ParsePositionInBoardGame(string line);

	void SetFirstTimeRenderCard(bool firstTimeRenderCard) { this->firstTimeRenderCard = firstTimeRenderCard; };

	LPSPRITE GetFontById(int id);

	void UpdateBoardGame();
	void RenderBoardGame();

	void RenderTimeUp();

	void SetIdCardForCardInBoard(int idCard);

	void SetWorldInBoardGame(int world);
	void SetSpeedInBoardGame(float speed);
	void SetCoinInBoardGame(int coin);
	void SetHeartInBoardGame(int heart);
	void SetPointInBoardGame(int point);
	void SetTimeInBoardGame(int time);

	static BoardGame* GetInstance();
};

