//#pragma once
//
//#include "Platform.h"
//
//// inheritance of CPlatform
//class CGround : public CPlatform
//{
//
//public:
//	CGround(float x, float y,
//    float cell_width, float cell_height, int length,
//        int sprite_id_begin, int sprite_id_middle, int sprite_id_end)
//		:CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end) {}
//
//	void Update(DWORD dt) {}
//	int IsDirectionColliable(float nx, float ny) { return 1; }
//};
//
//typedef CGround* LPGROUND;

#pragma once

#include "GameObject.h"

class CGround : public CGameObject
{
protected:
    int columns;
    int rows;
    int cellWidth;
    int cellHeight;

public:
    CGround(float x, float y, int columns, int rows, int cellWidth, int cellHeight)
        : CGameObject(x, y)
    {
        this->columns = columns;
        this->rows = rows;
        this->cellWidth = cellWidth;
        this->cellHeight = cellHeight;
    }

    void Render();
    void Update(DWORD dt) {} // don't update ground state
    void RenderBoundingBox();
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CGround* LPGROUND;