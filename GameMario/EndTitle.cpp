#include "EndTitle.h"
#include "debug.h"
#include "Textures.h"

void EndTitle::LoadEndTitleItem(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading end title items from : %s \n", filePath);

	ifstream f;
	f.open(filePath);


	char str[MAX_SHARED_RESOURCE_LINE];
	while (f.getline(str, MAX_SHARED_RESOURCE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines	
		ParseEndTitleItem(line);
	}

	f.close();
}

void EndTitle::ParseEndTitleItem(string line)
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
	LPSPRITE item = new Sprite(id, l, t, w + l, h + t, tex);
	this->endTitleItem.push_back(item);
}

void EndTitle::RenderEndTitle()
{
	if (GetTickCount64() - renderFinishedOneLine > 1000) {
		renderFinishedOneLine = GetTickCount64();
		currentLine++;
	}
	for (auto component : endTitleFontComponents) {
		for (auto item : endTitleItem) {
			if (item->GetId() == component->idFont && component->line <= currentLine) {
				item->Draw(component->x, component->y, 255);
			}
		}
	}
	for (auto item : endTitleItem) {
		if (item->GetId() == endTitleCardComponent->idFont && endTitleCardComponent ->line <= currentLine) {
			item->Draw(endTitleCardComponent->x, endTitleCardComponent->y, 255);
		}
	}
}

void EndTitle::LoadEndTitle(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading end title components from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	char str[MAX_SHARED_RESOURCE_LINE];
	while (f.getline(str, MAX_SHARED_RESOURCE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines	
		ParsePositionInEndTitle(line);
	}

	f.close();
}
void EndTitle::ParsePositionInEndTitle(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return;

	int type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	if (type == 1) {
		int idFont = atoi(tokens[3].c_str());
		int lineNumber = atoi(tokens[4].c_str());
		ComponentInEndTitle* componentEndTitle = new ComponentInEndTitle();
		componentEndTitle->x = x;
		componentEndTitle->y = y;
		componentEndTitle->idFont = idFont;
		componentEndTitle->line = lineNumber;
		endTitleFontComponents.push_back(componentEndTitle);
	}
	else if (type == 2) {
		int lineNumberX = atoi(tokens[3].c_str());
		endTitleCardComponent = new ComponentInEndTitle();
		endTitleCardComponent->x = x;
		endTitleCardComponent->y = y;
		endTitleCardComponent->idFont = 2;
		endTitleCardComponent->line = lineNumberX;
	}
}
EndTitle* EndTitle::__instance = NULL;
EndTitle* EndTitle::GetInstance()
{
	if (__instance == NULL) __instance = new EndTitle();
	return __instance;
}
