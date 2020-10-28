#include "PlayerState.h"

#include "AnimationDatabase.h"
#include "AnimationManager.h"
#include "Mario.h"
#include "debug.h"
PlayerState::PlayerState() {};


LPANIMATION PlayerState::GetAnimation() {
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	return animationDatabase->Get(animationID);
}