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
class BoardGame
{
	static BoardGame* __instance;
	vector<LPSPRITE> fonts;
	vector<LPSPRITE> cards;
	vector<ComponentPositionInBoard*> componentPositionInBoards;
	ComponentPositionInBoard* componentCardInBoards;
	Panel panelBoardGame;
public:
	void LoadBoardGameItems(LPCWSTR filePath);
	void LoadPositionInBoardGame(LPCWSTR filePath);
	void ParseInBoardBoardGame(string line);
	int GetFontNumber(int number);
	void ParseFontBoardGame(string line);
	void ParsePositionInBoardGame(string line);

	void UpdateBoardGame();
	void RenderBoardGame();

	void SetIdCardForCardInBoard(int idCard) { componentCardInBoards->idFontRender = idCard; };

	void SetWorldInBoardGame(int world);
	void SetSpeedInBoardGame(float speed);
	void SetCoinInBoardGame(int coin);
	void SetHeartInBoardGame(int heart);
	void SetPointInBoardGame(int point);
	void SetTimeInBoardGame(int time);

	static BoardGame* GetInstance();
};

