#pragma once
#include "GraphicsDefine.h"
#include <stdio.h>
#include "Mario.h"
#include "game.h"
#include "PlayScene.h"
#include "debug.h"
class Camera
{
	float cam_x = .0f;
	float cam_y = .0f;
	static Camera* __instance;
	bool canChangeCamY = false;
public:
	Camera();
	void SetLockCamY(bool canChange) { this->canChangeCamY = canChange; };
	void SetCamPos(float x, float y);
	void GetCamPos(float& x, float& y);
	void UpdateCamPos(DWORD dt);

	static Camera* GetInstance();
	~Camera();
};

typedef Camera* LPCAMERA;

