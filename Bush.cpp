#include "Bush.h"
#include "Textures.h"
#include "Game.h"

void CBush::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    CSprites::GetInstance()->Get(spriteId)->Draw(x + BUSH_SPRITE_WIDTH / 2, y + BUSH_SPRITE_HEIGHT / 2);
    //RenderBoundingBox();
}

void CBush::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    // top-left
    l = x;
    t = y;
    r = x + BUSH_SPRITE_WIDTH;
    b = y + BUSH_SPRITE_HEIGHT;
}

void CBush::RenderBoundingBox()
{
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)(r - l);
    rect.bottom = (int)(b - t);

    // center bounding box
    float xx = (l + r) / 2.0f;
    float yy = (t + b) / 2.0f;

    // get camera
    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    // draw bounding box with (centerX, centerY)
    auto bboxTex = CTextures::GetInstance()->Get(ID_TEX_BBOX);
    CGame::GetInstance()->Draw(
        xx - cx, yy - cy,
        bboxTex, nullptr,
        BBOX_ALPHA,
        rect.right, rect.bottom
    );
}
