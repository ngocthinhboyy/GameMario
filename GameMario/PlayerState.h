#pragma once
#include "Animation.h"
#include "MarioDefine.h"

class PlayerState
{
protected:
	int animationID;
public:
	PlayerState();
	
	virtual void SetAnimation(int levelPlayer) = 0;
	LPANIMATION GetAnimation();
	virtual void Update(int dt) {};
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode) {};
	virtual void OnKeyUp(int KeyCode) {};
	~PlayerState() {};
};

