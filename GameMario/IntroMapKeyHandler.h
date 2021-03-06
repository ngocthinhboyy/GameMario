#pragma once
#include "SceneKeyHandler.h"
#include "Scene.h"

class IntroMapKeyHandler : public SceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	IntroMapKeyHandler(Scene* s) :SceneKeyHandler(s) {};
};

