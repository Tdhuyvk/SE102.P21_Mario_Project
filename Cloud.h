#pragma once

#include "GameObject.h"
#include "Sprites.h"
#include "Animations.h"

#define CLOUD_SPRITE_WIDTH   16
#define CLOUD_SPRITE_HEIGHT  16

class CCloud : public CGameObject {
protected:
    int length;        // number of middle segments per row

public:
    // Constructor: x,y is top-left; spriteId for bush.txt (loaded in CSprites)
    CCloud(float x, float y, int length)
        : CGameObject(x, y)
    {
        this->length = length;
    }

    void Update(DWORD dt);

    void Render();

    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void RenderBoundingBox();

    // Always non-collidable
    int IsDirectionColliable(float nx, float ny) override { return 0; }
};

typedef CCloud* LPCLOUD;