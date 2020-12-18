#include "BoardGame.h"
#include "Camera.h"
#include "GameObject.h"
#include "BoardGameAndEndTitleDefine.h"
#include "Mario.h"
#include "PlayScene.h"

BoardGame* BoardGame::__instance = NULL;
void BoardGame::LoadBoardGameItems(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading board game items from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	int section = BOARDGAME_SECTION_UNKNOWN;

	char str[MAX_SHARED_RESOURCE_LINE];
	while (f.getline(str, MAX_SHARED_RESOURCE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;
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

	int section = BOARDGAME_SECTION_UNKNOWN;

	char str[MAX_SHARED_RESOURCE_LINE];
	while (f.getline(str, MAX_SHARED_RESOURCE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;
		ParsePositionInBoardGame(line);
	}
	LPCOMPONENTFONT component = new ComponentPositionInBoard();
	component->x = 288;
	component->y = 303;
	component->idFontRender = 34;
	component->idPosition = -1;
	componentTimeUp.push_back(component);
	component = new ComponentPositionInBoard();
	component->x = 313;
	component->y = 306;
	component->idFontRender = 23;
	component->idPosition = -1;
	componentTimeUp.push_back(component);
	component = new ComponentPositionInBoard();
	component->x = 338;
	component->y = 306;
	component->idFontRender = 27;
	component->idPosition = -1;
	componentTimeUp.push_back(component);
	component = new ComponentPositionInBoard();
	component->x = 363;
	component->y = 306;
	component->idFontRender = 19;
	component->idPosition = -1;
	componentTimeUp.push_back(component);
	component = new ComponentPositionInBoard();
	component->x = 413;
	component->y = 306;
	component->idFontRender = 35;
	component->idPosition = -1;
	componentTimeUp.push_back(component);
	component = new ComponentPositionInBoard();
	component->x = 438;
	component->y = 306;
	component->idFontRender = 30;
	component->idPosition = -1;
	componentTimeUp.push_back(component);
	f.close();
}
void BoardGame::ParseInBoardBoardGame(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return;

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
		return number;
	}
}
void BoardGame::ParseFontBoardGame(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return;

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

	if (tokens.size() < 4) return;

	int idPosition = atoi(tokens[0].c_str());
	float x = atoi(tokens[1].c_str());
	float y = atoi(tokens[2].c_str());
	int idFontRender = atoi(tokens[3].c_str());

	if (idPosition >= ID_CARD_1_IN_BOARD) {
		ComponentPositionInBoard* componentCard = new ComponentPositionInBoard();
		componentCard->x = x;
		componentCard->y = y;
		componentCard->idFontRender = idFontRender;
		componentCard->idPosition = idPosition;
		componentCardInBoard.push_back(componentCard);
		return;
	}

	ComponentPositionInBoard* componentPosition = new ComponentPositionInBoard();
	componentPosition->x = x;
	componentPosition->y = y;
	componentPosition->idPosition = idPosition;
	componentPosition->idFontRender = idFontRender;
	componentPositionInBoards.push_back(componentPosition);
}
LPSPRITE BoardGame::GetFontById(int id)
{
	for (auto x : fonts)
	{
		if (x->GetId() == id)
			return x;
	}
}
void BoardGame::UpdateBoardGame()
{
	Mario* mario = Mario::GetInstance();
	Scene* scene = Game::GetInstance()->GetCurrentScene();

	int world = 1;
	float speed = abs(mario->vx);
	int coin = mario->GetCoin();
	int heart = mario->GetHeart();
	int point = mario->GetPoint();
	int remainingTime = scene->GetRemainingTime();

	SetWorldInBoardGame(world);
	SetSpeedInBoardGame(speed);
	SetCoinInBoardGame(coin);
	SetHeartInBoardGame(heart);
	SetPointInBoardGame(point);
	SetTimeInBoardGame(remainingTime);
	
}
void BoardGame::RenderBoardGame()
{
	Camera* camera = Camera::GetInstance();
	float cam_x, cam_y;
	camera->GetCamPos(cam_x, cam_y);

	panelBoardGame.x = cam_x;
	panelBoardGame.y = cam_y + Game::GetInstance()->GetScreenHeight() - (BOARDGAME_HEIGHT - 2);
	panelBoardGame.h = BOARDGAME_HEIGHT;
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
	cards.at(0)->Draw(panelBoardGame.x + DEVIATION_X_WITH_BOARDGAME, panelBoardGame.y + DEVIATION_Y_WITH_BOARDGAME, 255);
	for (auto component : componentPositionInBoards) {
		for (auto font : fonts) {
			if (font->GetId() == component->idFontRender) {
				font->Draw(panelBoardGame.x + component->x + DEVIATION_X_WITH_BOARDGAME, panelBoardGame.y + component->y + DEVIATION_Y_WITH_BOARDGAME, 255);
			}
		}
	}
	for (auto component : componentCardInBoard) {
		if (!firstTimeRenderCard || component->idPosition != idCardRenderFirstTime) {
			for (auto card : cards) {
				if (card->GetId() == component->idFontRender) {
					card->Draw(panelBoardGame.x + component->x + DEVIATION_X_WITH_BOARDGAME, panelBoardGame.y + component->y + DEVIATION_Y_WITH_BOARDGAME, 255);
				}
			}
		}
		else {
			if (GetTickCount64() - timeRenderCard >= TIME_LIMIT_CHANGE_RENDER_CARD) {
				timeRenderCard = GetTickCount64();
				renderCardInWhenFirstTime = !renderCardInWhenFirstTime;
			}
			if (renderCardInWhenFirstTime) {
				for (auto card : cards) {
					if (card->GetId() == component->idFontRender) {
						card->Draw(panelBoardGame.x + component->x + DEVIATION_X_WITH_BOARDGAME, panelBoardGame.y + component->y + DEVIATION_Y_WITH_BOARDGAME, 255);
					}
				}
			}
		}
	}
}
void BoardGame::RenderTimeUp()
{
	float cam_x, cam_y;
	Camera::GetInstance()->GetCamPos(cam_x,cam_y);
	for (auto x : componentTimeUp) {
		for (auto font : fonts) {
			if (font->GetId() == x->idFontRender) {
				font->Draw(cam_x + x->x, cam_y+ x->y, 255);
			}
		}
	}
}
void BoardGame::SetIdCardForCardInBoard(int idCard)
{
	for (auto component : componentCardInBoard) {
		if (component->idFontRender == 0) {
			component->idFontRender = idCard;
			firstTimeRenderCard = true;
			timeRenderCard = GetTickCount64();
			idCardRenderFirstTime = component->idPosition;
			return;
		}
	}
}
void BoardGame::SetWorldInBoardGame(int world)
{
	componentPositionInBoards.at(ID_POSITION_OF_WORLDNUMBER)->idFontRender = GetFontNumber(world);
}
void BoardGame::SetSpeedInBoardGame(float speed)
{
	if (Mario::GetInstance()->GetIsFlying()) {
		if (firstTimeTurnOnMaxSpeed) {
			firstTimeTurnOnMaxSpeed = false;
			renderWhiteP = true;
			timeRenderWhite = GetTickCount64();
		}
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_WHITE;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_WHITE;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_WHITE;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_WHITE;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_WHITE;
		if (renderWhiteP) {
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = GetFontNumber(44);
			if (GetTickCount64() - timeRenderWhite >= TIME_LIMIT_CHANGE_RENDER_SPEED) {
				renderWhiteP = false;
				timeRenderWhite = GetTickCount64();
			}
		}
		else {
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = GetFontNumber(45);
			if (GetTickCount64() - timeRenderWhite >= TIME_LIMIT_CHANGE_RENDER_SPEED) {
				renderWhiteP = true;
				timeRenderWhite = GetTickCount64();
			}
		}
		return;
	}
	if (Mario::GetInstance()->GetIsRunning()) {
		if (speed >= SPEED_LEVEL_MAX) {
			if (firstTimeTurnOnMaxSpeed) {
				firstTimeTurnOnMaxSpeed = false;
				renderWhiteP = true;
				timeRenderWhite = GetTickCount64();
			}
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_WHITE;
			if (renderWhiteP) {
				componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_WHITE;
				if (GetTickCount64() - timeRenderWhite >= TIME_LIMIT_CHANGE_RENDER_SPEED) {
					renderWhiteP = false;
					timeRenderWhite = GetTickCount64();
				}
			}
			else {
				componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
				if (GetTickCount64() - timeRenderWhite >= TIME_LIMIT_CHANGE_RENDER_SPEED) {
					renderWhiteP = true;
					timeRenderWhite = GetTickCount64();
				}
			}
		}
		else if (speed >= SPEED_LEVEL_6) {
			firstTimeTurnOnMaxSpeed = true;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
		}
		else if (speed >= SPEED_LEVEL_5) {
			firstTimeTurnOnMaxSpeed = true;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_WHITE;

			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
		}
		else if (speed >= SPEED_LEVEL_4) {
			firstTimeTurnOnMaxSpeed = true;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_WHITE;

			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
		}
		else if (speed >= SPEED_LEVEL_3) {
			firstTimeTurnOnMaxSpeed = true;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_WHITE;

			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
		}
		else if (speed >= SPEED_LEVEL_2) {
			firstTimeTurnOnMaxSpeed = true;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_WHITE;

			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
		}
		else if (speed >= SPEED_LEVEL_1) {
			firstTimeTurnOnMaxSpeed = true;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_WHITE;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
		}
		else {
			firstTimeTurnOnMaxSpeed = true;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_BLACK;
			componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
		}
	}
	else {
		firstTimeTurnOnMaxSpeed = true;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_1)->idFontRender = ID_FONT_ARROW_BLACK;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_2)->idFontRender = ID_FONT_ARROW_BLACK;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_3)->idFontRender = ID_FONT_ARROW_BLACK;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_4)->idFontRender = ID_FONT_ARROW_BLACK;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_5)->idFontRender = ID_FONT_ARROW_BLACK;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_6)->idFontRender = ID_FONT_ARROW_BLACK;
		componentPositionInBoards.at(ID_POSITION_OF_SPEED_LEVEL_MAX)->idFontRender = ID_FONT_P_TAG_BLACK;
	}
}
void BoardGame::SetCoinInBoardGame(int coin)
{
	componentPositionInBoards.at(ID_POSITION_OF_COIN_2)->idFontRender = GetFontNumber(coin % 10);
	coin /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_COIN_1)->idFontRender = GetFontNumber(coin % 10);
}
void BoardGame::SetHeartInBoardGame(int heart)
{
	componentPositionInBoards.at(ID_POSITION_OF_HEART)->idFontRender = GetFontNumber(heart);
}
void BoardGame::SetPointInBoardGame(int point)
{
	componentPositionInBoards.at(ID_POSITION_OF_POINT_7)->idFontRender = GetFontNumber(point % 10);
	point /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_POINT_6)->idFontRender = GetFontNumber(point % 10);
	point /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_POINT_5)->idFontRender = GetFontNumber(point % 10);
	point /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_POINT_4)->idFontRender = GetFontNumber(point % 10);
	point /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_POINT_3)->idFontRender = GetFontNumber(point % 10);
	point /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_POINT_2)->idFontRender = GetFontNumber(point % 10);
	point /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_POINT_1)->idFontRender = GetFontNumber(point % 10);
}
void BoardGame::SetTimeInBoardGame(int time)
{
	componentPositionInBoards.at(ID_POSITION_OF_TIME_3)->idFontRender = GetFontNumber(time % 10);
	time /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_TIME_2)->idFontRender = GetFontNumber(time % 10);
	time /= 10;
	componentPositionInBoards.at(ID_POSITION_OF_TIME_1)->idFontRender = GetFontNumber(time % 10);
}
BoardGame* BoardGame::GetInstance()
{
	if (__instance == NULL) __instance = new BoardGame();
	return __instance;
}
