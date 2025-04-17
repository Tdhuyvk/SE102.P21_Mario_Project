#pragma once
#include "GameObject.h"

// Box color
enum BoxColor {
    BOX_WHITE,
    BOX_GREEN,
    BOX_PINK,
    BOX_BLUE
};

class CBox : public CGameObject
{
protected:
    int columns;        // cell
    int rows;           // cell
    float cellWidth;    // pixel or game unit
    float cellHeight;   
    BoxColor color;   // box color (asset IDs)

public:
    CBox(float x, float y, int columns, int rows, float cellWidth, float cellHeight, BoxColor color)
        : CGameObject(x, y)
    {
        this->columns = columns;
        this->rows = rows;
        this->cellWidth = cellWidth;
        this->cellHeight = cellHeight;
        this->color = color;
    }

    void Render();
    void Update(DWORD dt) {} // don't update box state
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void RenderBoundingBox();
};

typedef CBox* LPBOX;
