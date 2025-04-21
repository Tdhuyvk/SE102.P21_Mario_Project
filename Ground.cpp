#include "Ground.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
#include "AssetIDs.h"

int GetBoxSpriteId(int section, int pos)
{
    // section: 0 - head, 1 - body
    // pos: 0 - left, 1 - mid, 2 - right
    if (section == 0) // head
    {
        if (pos == 0) return ID_SPRITE_GROUND_TOP_LEFT;
        else if (pos == 1) return ID_SPRITE_GROUND_TOP_CEN;
        else if (pos == 2) return ID_SPRITE_GROUND_TOP_RIGHT;
    }
    else if (section == 1) // body
    {
        if (pos == 0) return ID_SPRITE_GROUND_MID_LEFT;
        else if (pos == 1) return ID_SPRITE_GROUND_MID_CEN;
        else if (pos == 2) return ID_SPRITE_GROUND_MID_RIGHT;
    }

    return 0;
}

void CGround::Render() {
    /*float l, t, r, b;
    GetBoundingBox(l, t, r, b);*/

    float l = x + cellWidth / 2;
    float t = y + cellHeight / 2;

    CSprites* sprites = CSprites::GetInstance();
    for (int row = 0; row < rows; ++row) {
        int section = (row == 0) ? 0 : 1;
        for (int col = 0; col < columns; ++col) {
            int pos = (col == 0) ? 0
                : (col == columns - 1) ? 2
                : 1;
            int id = GetBoxSpriteId(section, pos);
            sprites->Get(id)->Draw(
                l + col * cellWidth,
                t + row * cellHeight
            );
        }
    }
    RenderBoundingBox();
}

void CGround::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x;
    t = y;
    r = x + columns * cellWidth;
    b = y + rows * cellHeight;
}

void CGround::RenderBoundingBox()
{
    // get bounding box
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    // RECT (r-l) x (b-t)
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)(r - l);
    rect.bottom = (int)(b - t);

    // center bounding?box
    float xx = (l + r) / 2;
    float yy = (t + b) / 2;

    /*long width = (long)(r - l);
    long height = (long)(b - t);

    float drawX = l + width * 0.5f - cellWidth * 0.5f;
    float drawY = t + height * 0.5f - cellHeight * 0.5f;*/

    // get camera
    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    /*float xx = l + rect.right / 2;
    float yy = t + rect.bottom / 2;*/

    /*float xx = l + rect.right / 2 - 8;
    float yy = t + rect.bottom / 2 - 8;*/

   /* CGame::GetInstance()->Draw(xx - cx, yy - cy,
        CTextures::GetInstance()->Get(ID_TEX_BBOX),
        nullptr, BBOX_ALPHA, rect.right, rect.bottom);*/

    // draw bounding box with (centerX, centerY)
    CGame::GetInstance()->Draw(xx - cx, yy - cy,
        CTextures::GetInstance()->Get(ID_TEX_BBOX),
        nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}
