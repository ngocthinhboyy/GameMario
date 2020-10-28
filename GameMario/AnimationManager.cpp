#include "AnimationManager.h"
#include "debug.h"

AnimationManager* AnimationManager::__instance = NULL;

AnimationManager::AnimationManager()
{

}

AnimationManager* AnimationManager::GetInstance()
{
	if (__instance == NULL) __instance = new AnimationManager();
	return __instance;
}

LPANIMATION_SET AnimationManager::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_manager[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void AnimationManager::Add(int id, LPANIMATION_SET ani_set)
{
	DebugOut(L"[INFO] animation database added: %d \n", id);
	animation_manager[id] = ani_set;
}
