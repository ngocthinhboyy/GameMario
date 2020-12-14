#include "WorldMapObject.h"
#include "WorldMapDefine.h"
#include "AnimationDatabase.h"
#include "debug.h"

WorldMapObject::WorldMapObject()
{
}

WorldMapObject::WorldMapObject(float x, float y, int w, int h, int type)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->noCollisionConsideration = true;
	this->typeWorldMapObject = type;
	if (typeWorldMapObject == WORLDMAP_OBJECT_TYPE_GUARD) {
		vx = 0.05f;
		timeMoveForGuard = GetTickCount64();
	}
}

void WorldMapObject::Render()
{
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	switch (typeWorldMapObject)
	{
		case WORLDMAP_OBJECT_TYPE_CACTUS:
		{
			animation = AnimationDatabase::GetInstance()->Get(CACTUS_ANI);
			break;
		}
		case WORLDMAP_OBJECT_TYPE_HELP_TAGS:
		{
			animation = AnimationDatabase::GetInstance()->Get(HELP_TAGS_ANI);
			break;
		}
		case WORLDMAP_OBJECT_TYPE_GUARD:
		{
			if(nx < 0)
				scale = D3DXVECTOR2(RATIO_X_FLIP_SCALE, RATIO_Y_SCALE);
			animation = AnimationDatabase::GetInstance()->Get(GUARD_ANI);
			break;
		}
	default:
		break;
	}
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void WorldMapObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void WorldMapObject::Update(DWORD dt, int scaleTime)
{
	if (typeWorldMapObject == WORLDMAP_OBJECT_TYPE_GUARD) {
		if (GetTickCount64() - timeMoveForGuard >= 1000) {
			vx = -vx;
			nx = -nx;
			timeMoveForGuard = GetTickCount64();
		}
		dx = vx * dt;
		x += dx;
	}
}

void WorldMapObject::RenderBoundingBox()
{
}
