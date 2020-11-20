#include "BoardGame.h"
#include "Camera.h"
#include "GameObject.h"

BoardGame* BoardGame::__instance = NULL;
void BoardGame::LoadBoardGameItems(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading board game items from : %s \n", filePath);

	ifstream f;
	f.open(filePath);


	// current resource section flag
	int section = BOARDGAME_SECTION_UNKNOWN;

	char str[MAX_SHARED_RESOURCE_LINE];
	while (f.getline(str, MAX_SHARED_RESOURCE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[INBOARD]") { section = BOARDGAME_SECTION_INBOARD; continue; }
		if (line == "[FONT]") { section = BOARDGAME_SECTION_FONT; continue; }
		if (line[0] == '[') { section = BOARDGAME_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case BOARDGAME_SECTION_INBOARD: {ParseInBoardBoardGame(line); break; }
		case BOARDGAME_SECTION_FONT: {ParseFontBoardGame(line); break; }
		default:
			break;
		}
	}

	f.close();
}
void BoardGame::LoadPositionInBoardGame(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading board game position from : %s \n", filePath);

	ifstream f;
	f.open(filePath);


	// current resource section flag
	int section = BOARDGAME_SECTION_UNKNOWN;

	char str[MAX_SHARED_RESOURCE_LINE];
	while (f.getline(str, MAX_SHARED_RESOURCE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines	
		ParsePositionInBoardGame(line);
	}

	f.close();
}
void BoardGame::ParseInBoardBoardGame(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int w = atoi(tokens[3].c_str());
	int h = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->GetTexture(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}
	LPSPRITE card = new Sprite(id, l, t, w + l , h + t, tex);
	this->cards.push_back(card);
}
int BoardGame::GetFontNumber(int number)
{
	switch (number) {
	case 0:
		return 5;
	case 1:
		return 6;
	case 2:
		return 7;
	case 3:
		return 8;
	case 4:
		return 9;
	case 5:
		return 10;
	case 6:
		return 11;
	case 7:
		return 12;
	case 8:
		return 13;
	case 9:
		return 14;
	default:
		break;
	}
}
void BoardGame::ParseFontBoardGame(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int w = atoi(tokens[3].c_str());
	int h = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->GetTexture(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}
	LPSPRITE font = new Sprite(id, l, t, w + l, h + t, tex);
	this->fonts.push_back(font);
}
void BoardGame::ParsePositionInBoardGame(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	float x = atoi(tokens[0].c_str());
	float y = atoi(tokens[1].c_str());
	int id = atoi(tokens[2].c_str());

	ComponentPositionInBoard* componentPosition = new ComponentPositionInBoard();
	componentPosition->x = x;
	componentPosition->y = y;
	componentPosition->id = id;
	DebugOut(L"ID %d\n", componentPosition->id);
	componentPositionInBoards.push_back(componentPosition);
}
void BoardGame::RenderBoardGame()
{
	Camera* camera = Camera::GetInstance();
	float cam_x, cam_y;
	camera->GetCamPos(cam_x, cam_y);
	// cam x = -15 745
	// cam y = -38 692
	panelBoardGame.x = cam_x;
	panelBoardGame.y = cam_y + Game::GetInstance()->GetScreenHeight() - 116;
	panelBoardGame.h = 116;
	panelBoardGame.w = Game::GetInstance()->GetScreenWidth();
	D3DXVECTOR3 p(panelBoardGame.x - cam_x, panelBoardGame.y - cam_y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->GetTexture(ID_TEX_BOARDGAME);

	float l, t, r, b;

	rect.left = 0;
	rect.top = 0;
	rect.right = panelBoardGame.w;
	rect.bottom = panelBoardGame.h;

	Game::GetInstance()->Draw(panelBoardGame.x, panelBoardGame.y, bbox, rect.left, rect.top, rect.right, rect.bottom, 255);
	cards.at(0)->Draw(panelBoardGame.x + 20, panelBoardGame.y + 10, 255);
	for (auto component : componentPositionInBoards) {
		for (auto font : fonts) {
			if (font->GetId() == component->id) {
				
				font->Draw(panelBoardGame.x + component->x +20, panelBoardGame.y + component->y +10, 255);
			}
		}
	}
}
BoardGame* BoardGame::GetInstance()
{
	if (__instance == NULL) __instance = new BoardGame();
	return __instance;
}
