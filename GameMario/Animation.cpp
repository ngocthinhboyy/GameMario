#include "Animation.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "debug.h"
#include "PlayScene.h"

void Animation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = SpriteManager::GetInstance()->GetSprite(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new AnimationFrame(sprite, t);
	frames.push_back(frame);
}

//bool Animation::isLastFrame = false;
void Animation::Render(float x, float y, int alpha, D3DXVECTOR2 scale, int offset)
{
	DWORD now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) {
				isLastFrame = true;
				currentFrame = 0;
			}
			else
				isLastFrame = false;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha, scale, offset);
}

void Animation::ResetAnimation()
{
	isLastFrame = false;
	currentFrame = -1;

}

