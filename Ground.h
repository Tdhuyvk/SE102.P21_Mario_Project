#pragma once

#include "GameObject.h"

// 
// The most used type of object in Mario! 
//

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