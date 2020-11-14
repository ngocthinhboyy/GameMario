#include <fstream>
#include <iostream>
#include "ResourcesSharedLoad.h"
#include "Textures.h"
#include "debug.h"
#include "SpriteManager.h"
#include "AnimationDatabase.h"
#include "AnimationManager.h"
ResourcesSharedLoad::ResourcesSharedLoad()
{
}

void ResourcesSharedLoad::ParseTextureShared(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->AddTexture(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void ResourcesSharedLoad::ParseSpriteShared(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
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

	SpriteManager::GetInstance()->AddSprite(ID, l, t, w + l, h + t, tex);
}

void ResourcesSharedLoad::ParseAnimationShared(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	AnimationDatabase::GetInstance()->Add(ani_id, ani);
}

void ResourcesSharedLoad::ParseAnimationSetShared(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());
	DebugOut(L"ani_set %d\n", ani_set_id);

	LPANIMATION_SET s = new AnimationSet();

	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animationDatabase->Get(ani_id);
		s->push_back(ani);
	}

	AnimationManager::GetInstance()->Add(ani_set_id, s);
}

void ResourcesSharedLoad::LoadSharedResource(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading shared resources from : %s \n", filePath);

	ifstream f;
	f.open(filePath);


	// current resource section flag
	int section = SHARED_RESOURCE_SECTION_UNKNOWN;

	char str[MAX_SHARED_RESOURCE_LINE];
	while (f.getline(str, MAX_SHARED_RESOURCE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SHARED_RESOURCE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SHARED_RESOURCE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SHARED_RESOURCE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SHARED_RESOURCE_SECTION_ANIMATION_SETS; continue;
		}
		if (line[0] == '[') { section = SHARED_RESOURCE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SHARED_RESOURCE_SECTION_TEXTURES: {ParseTextureShared(line); break; }
		case SHARED_RESOURCE_SECTION_SPRITES: {ParseSpriteShared(line); break; }
		case SHARED_RESOURCE_SECTION_ANIMATIONS: {ParseAnimationShared(line); break; }
		case SHARED_RESOURCE_SECTION_ANIMATION_SETS: {ParseAnimationSetShared(line); break; }
		default:
			break;
		}
	}

	f.close();
}

ResourcesSharedLoad::~ResourcesSharedLoad()
{
}
