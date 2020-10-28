#include "AnimationDatabase.h"
#include "debug.h"
AnimationDatabase* AnimationDatabase::__instance = NULL;

AnimationDatabase* AnimationDatabase::GetInstance()
{
	if (__instance == NULL) __instance = new AnimationDatabase();
	return __instance;
}

void AnimationDatabase::Add(int id, LPANIMATION ani)
{
	DebugOut(L"[INFO] animation added: %d \n", id);
	animations[id] = ani;
}

LPANIMATION AnimationDatabase::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void AnimationDatabase::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}
