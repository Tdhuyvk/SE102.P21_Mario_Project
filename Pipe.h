#pragma once

#include "GameObject.h"

// 
// Create pipe type of object in Mario! 
// 
class CPipe : public CGameObject
{
protected:
	int height;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdTop, spriteIdBottom;

public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int height,
		int sprite_id_top, int sprite_id_bottom) :CGameObject(x, y)
	{
		this->height = height;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdTop = sprite_id_top;
		this->spriteIdBottom = sprite_id_bottom;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CPipe* LPPPIPE;