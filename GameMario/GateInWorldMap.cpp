#include "GateInWorldMap.h"
#include "WorldMapDefine.h"
#include "AnimationDatabase.h"
#include "Mario.h"
#include "debug.h"

GateInWorldMap::GateInWorldMap()
{
}

GateInWorldMap::GateInWorldMap(float x, float y, int w, int h, int type, bool isInvisibleGate, int directionX, int directionY)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->isInvisibleGate = isInvisibleGate;
	this->noCollisionConsideration = true;
	this->type = type;
	this->directionX = directionX;
	this->directionY = directionY;
	this->gameObjectID = idGenerate++;
}

void GateInWorldMap::Render()
{
	if (isInvisibleGate)
		return;
	int alpha = 255;
	D3DXVECTOR2 scale;
	scale = D3DXVECTOR2(RATIO_X_SCALE, RATIO_Y_SCALE);
	switch (type)
	{
	case TYPE_GATE_START:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_START_ANI);
		break;
	}
	case TYPE_GATE_1:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_1_ANI);
		break;
	}
	case TYPE_GATE_2:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_2_ANI);
		break;
	}
	case TYPE_GATE_3:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_3_ANI);
		break;
	}
	case TYPE_GATE_4:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_4_ANI);
		break;
	}
	case TYPE_GATE_5:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_5_ANI);
		break;
	}
	case TYPE_GATE_6:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_6_ANI);
		break;
	}
	case TYPE_GATE_CARD:
	{
		animation = AnimationDatabase::GetInstance()->Get(GATE_CARD_ANI);
		break;
	}
	default:
		break;
	}
	if (animation != NULL) {
		animation->Render(x, y, alpha, scale);
	}
}

void GateInWorldMap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

int GateInWorldMap::CheckMarioIsStandingOn()
{
	float leftBB, rightBB, topBB, bottomBB = .0f;
	float leftBBMario, rightBBMario, topBBMario, bottomBBMario = .0f;
	GetBoundingBox(leftBB, topBB, rightBB, bottomBB);
	Mario::GetInstance()->GetBoundingBox(leftBBMario, topBBMario, rightBBMario, bottomBBMario);
	rightBBMario = leftBBMario + 48;
	bottomBBMario = topBBMario + 48;
	float widthBB = rightBB - leftBB;
	float heightBB = bottomBB - topBB;
	float widthMario = rightBBMario - leftBBMario;
	float heightMario = bottomBBMario - topBBMario;

	if ((leftBB + widthBB >= leftBBMario) && (leftBBMario + widthMario >= leftBB) && (topBB + heightBB >= topBBMario) && (topBBMario + heightMario >= topBB)) {
		return gameObjectID;
	}
	return -1;
}

void GateInWorldMap::RenderBoundingBox()
{
}
