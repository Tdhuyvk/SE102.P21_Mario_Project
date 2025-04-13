#pragma once
#include "GameObject.h"

// Block color
enum BlockColor {
    BLOCK_WHITE,
    BLOCK_GREEN,
    BLOCK_PINK,
    BLOCK_BLUE
};

class CBlock : public CGameObject
{
protected:
    int columns;        // cell
    int rows;           // cell
    float cellWidth;    // pixel or game unit
    float cellHeight;   
    BlockColor color;   // block color (asset IDs)

public:
    CBlock(float x, float y, int columns, int rows, float cellWidth, float cellHeight, BlockColor color)
        : CGameObject(x, y)
    {
        this->columns = columns;
        this->rows = rows;
        this->cellWidth = cellWidth;
        this->cellHeight = cellHeight;
        this->color = color;
    }

    void Render();
    void Update(DWORD dt) {} // don't update block state
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void RenderBoundingBox();
};

typedef CBlock* LPBLOCK;
