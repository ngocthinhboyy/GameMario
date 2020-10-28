#pragma once
#include "KeyEventHandler.h"

#include "Scene.h"
class SceneKeyHandler : public KeyEventHandler
{
protected:
	LPSCENE scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	SceneKeyHandler(LPSCENE s) :KeyEventHandler() { scence = s; }
};

