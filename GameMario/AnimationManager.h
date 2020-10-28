#pragma once
#include "Animation.h"
class AnimationManager
{
	static AnimationManager* __instance;

	unordered_map<int, LPANIMATION_SET> animation_manager;

public:
	AnimationManager();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static AnimationManager* GetInstance();
};

