#pragma once
#include "KeyEventHandler.h"
#include <d3dx9.h>
class Scene
{
protected:
	LPKEYEVENTHANDLER key_handler;
	int id;
	LPCWSTR sceneFilePath;

public:
	Scene(int id, LPCWSTR filePath);

	LPKEYEVENTHANDLER GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};
typedef Scene* LPSCENE;

