#pragma once
#include <unordered_map>
#include "Animation.h"
class AnimationDatabase
{
	static AnimationDatabase* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static AnimationDatabase* GetInstance();
};

