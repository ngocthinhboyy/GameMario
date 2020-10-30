#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "AnimationFrame.h"
#include "GraphicsDefine.h"

using namespace std;

class Animation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
	bool isLastFrame = false;
public:
	Animation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);

	void Render(float x, float y, int alpha = 255, D3DXVECTOR2 scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE));
	bool GetIsLastFrame() { return this->isLastFrame; };
	int GetCurrentFrame() { return this->currentFrame; };
	void ResetAnimation();
};
typedef Animation* LPANIMATION;
typedef vector<LPANIMATION> AnimationSet;
typedef AnimationSet* LPANIMATION_SET;
