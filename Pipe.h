#pragma once

#include "GameObject.h"

// 
// Create pipe type of object in Mario! 
// 
class CPipe : public CGameObject
{
protected:
	int height;				// Unit: cell
	int type;				// type: 0 -> pipe up, 1 -> pipe down
	float cellWidth;
	float cellHeight;
	int spriteIdTop, spriteIdBottom;

public:
	CPipe(float x, float y, int height, int type);

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CPipe* LPPPIPE;