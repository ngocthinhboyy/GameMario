#pragma once
#include "GameObject.h"
class WorldMapObject : public GameObject
{
	int typeWorldMapObject = 0;
	LPANIMATION animation;
public:
	WorldMapObject();
	WorldMapObject(float x, float y, int w, int h, int type);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void RenderBoundingBox();
};

typedef WorldMapObject* LPWORLDMAPOBJECT;

