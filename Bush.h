#pragma once

#include "GameObject.h"
#include "Sprites.h"
#include "Animations.h"

#define BUSH_SPRITE_WIDTH   16
#define BUSH_SPRITE_HEIGHT  16

class CBush : public CGameObject {
private:
    int spriteId;      // ID sprite (ID_SPRITE_BUSH_SMALL, ID_SPRITE_BUSH_BIG_TOP_LEFT,…)

public:
    // Constructor: x,y is top-left; spriteId for bush.txt (loaded in CSprites)
    CBush(float x, float y, int spriteId)
        : CGameObject(x, y), spriteId(spriteId) {}

    void Update(DWORD dt);

    void Render();

    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void RenderBoundingBox();
};
