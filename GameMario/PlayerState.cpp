#include "PlayerState.h"

#include "AnimationDatabase.h"
#include "AnimationManager.h"
#include "Mario.h"
#include "debug.h"
PlayerState::PlayerState() {};

int PlayerState::animationID = 0;
LPANIMATION PlayerState::GetAnimation() {
	AnimationDatabase* animationDatabase = AnimationDatabase::GetInstance();
	return animationDatabase->Get(animationID);
}